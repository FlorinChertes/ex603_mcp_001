#include <algorithm>
#include <map>
#include <vector>
#include <ranges>
#include <set>
#include <string>
#include <iostream>

#include <memory>
#include <cassert>

// Some non-refined, abstract helper functions:
template <class Rng1, class Rng2>
bool SubsetOf(Rng1&& r1, Rng2&& r2)
{
	return std::ranges::all_of (std::forward<Rng1>(r1),
		[&r2](const auto & el) {
            return std::ranges::find(r2, el) != std::ranges::end(r2);
        }
    );
}

template <class Rng1, class Rng2>
bool Equal(Rng1&& r1, Rng2&& r2) // We could serialize r1, r2 into vectors and sort them.
{
	return SubsetOf (std::forward<Rng1>(r1), std::forward<Rng2>(r2))
        && SubsetOf (std::forward<Rng2>(r2), std::forward<Rng1>(r1));
}

struct ToVectorFn
{
    template <class Rng>
    auto operator()(Rng&& r) const
    {
        return std::vector<std::ranges::range_value_t<Rng>>{
            std::ranges::begin(r),
            std::ranges::end(r)
        };
    }
};

constexpr ToVectorFn ToVector;

template <class R>
auto operator|(R&& r, const ToVectorFn& f)
{
    return std::invoke(f, std::forward<R>(r));
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CStpEl
{
public:
    void set_name(const std::string& name) { m_name = name; }
    const std::string& get_name() const { return m_name; }

    virtual void show() = 0;

    virtual ~CStpEl() { std::cout << "dtor CStpEl " << m_name << std::endl; }
private:
    std::string m_name;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CLe : public CStpEl
{
public:
    explicit CLe(const std::string& name) { set_name(name); }
    ~CLe() { std::cout << "dtor Le " << get_name() << std::endl; }

    void show() override { std::cout << get_name() << std::endl; }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CKla : public CStpEl
{
public:
    explicit CKla(const std::string& name) { set_name(name); }
    ~CKla() { std::cout << "dtor Kla " << get_name() << std::endl; }

    void show() override { std::cout << get_name() << std::endl; }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct CKoppZeile
{
public:
    void set_element(CStpEl* elem) { m_arStpEl.push_back(elem); }

    bool check_one_lehrer_pro_KZ() const;
    CLe* get_lehrer() const;
    bool check_all_klassen_gleich(const CKoppZeile& other) const;
    std::vector<std::string> get_all_klassen_name() const;

//private:
    // Alle Elemente, die gemeinsam während eines Unterrichts belegt sind:
    std::vector<CStpEl*> m_arStpEl;
};

template <class Target>
struct CastableToFn
{
	template <class Source>
	bool operator()(const Source* s) const noexcept
	{
		return dynamic_cast<const Target*>(s) != nullptr;
	}
};
template <class Target>
constexpr auto CastableTo = CastableToFn<Target>{};

bool CKoppZeile::check_one_lehrer_pro_KZ() const {
	return std::count_if(m_arStpEl.begin(), m_arStpEl.end(), CastableTo<CLe>) == 1;
}

CLe* CKoppZeile::get_lehrer() const {
	auto it = std::find_if(m_arStpEl.begin(), m_arStpEl.end(), CastableTo<CLe>);
	return it != m_arStpEl.end() ? static_cast<CLe*>(*it) : nullptr;
}

bool CKoppZeile::check_all_klassen_gleich(const CKoppZeile& other) const
{
	return Equal(
		m_arStpEl | std::views::filter(CastableTo<CKla>),
		other.m_arStpEl | std::views::filter(CastableTo<CKla>));
}

std::vector<std::string> CKoppZeile::get_all_klassen_name() const
{
	return m_arStpEl | std::views::filter(CastableTo<CKla>)
		| std::views::transform([](CStpEl* pEl) { return static_cast<CKla*>(pEl)->get_name(); })
		| ToVector;	// Custom function that converts ranges to vector;
						// As an alternative, use vector-from-iterator-pair constructor.
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class CUnt
{
public:
        CUnt() = default;

        CUnt(const CUnt& unt)
        {
			*this = unt;
        }

		CUnt& operator=(const CUnt& unt)
		{
			Clear();
            for (CKoppZeile* pKoppZeile : unt.m_arKoppZeile)
            {
                    m_arKoppZeile.push_back(new CKoppZeile(*pKoppZeile));
            }
            return *this;
		}

        ~CUnt()
        {
			Clear();
        }

        void AddKoppZeile(CKoppZeile* pKoppZeile)
        {
            m_arKoppZeile.push_back(pKoppZeile);
        }

		void Clear()
		{
            for (CKoppZeile* pKoppZeile : m_arKoppZeile)
            {
                    delete pKoppZeile;
            }
			m_arKoppZeile = {};
		}

        bool check_one_lehrer_pro_KZ() const;
        bool check_one_lehrer_for_all_KZ() const;
        bool if_one_klass_gleich_all_kalssen_gleich() const;

private:
        std::vector<CKoppZeile*> m_arKoppZeile;
};

bool CUnt::check_one_lehrer_pro_KZ() const {
	return std::ranges::all_of(m_arKoppZeile, &CKoppZeile::check_one_lehrer_pro_KZ);
}

bool CUnt::check_one_lehrer_for_all_KZ() const
{
	auto allTeachers = m_arKoppZeile
        | std::views::transform([](CKoppZeile* pZ) { return pZ->get_lehrer(); })
		| ToVector;
	std::sort(allTeachers.begin(), allTeachers.end());
	return std::adjacent_find(allTeachers.begin(), allTeachers.end()) == allTeachers.end();
}

bool CUnt::if_one_klass_gleich_all_kalssen_gleich() const
{
    std::map<const CStpEl*, const CKoppZeile*> klaToKoppZeile;
	for (const CKoppZeile* pZ : m_arKoppZeile)
	{
		std::set<const CKoppZeile*> relatedCouplingLines;
		for (const CStpEl* pEl : pZ->m_arStpEl | std::views::filter(CastableTo<CKla>))
		{
			auto r = klaToKoppZeile.try_emplace(pEl, pZ);
			if (!std::get<1>(r)) // There is already an element
			{
				relatedCouplingLines.insert(std::get<0>(r)->second); // The already inserted element.
			}
		}
		if (std::ranges::any_of(relatedCouplingLines, [pZ](const CKoppZeile* pOther) { return !pZ->check_all_klassen_gleich(*pOther); }))
		{
			return false;
		}
	}

	return true;
}


CLe lehrer_1("L1");
CLe lehrer_2("L2");
CLe lehrer_3("L3");
CLe lehrer_4("L4");

CKla klasse_1("K1");
CKla klasse_2("K2");

CKla klasse_3("K3");
CKla klasse_4("K4");
CKla klasse_5("K5");
CKla klasse_6("K6");

CUnt build_unterricht()
{
    CKoppZeile k_zeile_1;
    k_zeile_1.set_element(&lehrer_1);
    k_zeile_1.set_element(&klasse_1);

    //      1. in jeder Kopplungszeile höchstens 1 Lehrer vorkommt
    //k_zeile_1.set_element(&lehrer_3);

    CKoppZeile k_zeile_2;
    k_zeile_2.set_element(&lehrer_2);
    k_zeile_2.set_element(&klasse_2);

    CUnt unt;

    unt.AddKoppZeile(new CKoppZeile(k_zeile_1));
    unt.AddKoppZeile(new CKoppZeile(k_zeile_2));

    //      2. jeder Lehrer im Unterricht höchstens 1-mal vorkommt und
    /*
    CKoppZeile k_zeile_3;
    k_zeile_3.set_element(&lehrer_2);
    k_zeile_3.set_element(&klasse_3);
    unt.AddKoppZeile(new CKoppZeile(k_zeile_3));
    //*/

    //      3. wenn zwei Kopplungszeilen eine Klasse gemeinsam haben, dann
    //              sind alle Klassen gleich.
    CKoppZeile k_zeile_4;
    k_zeile_4.set_element(&lehrer_3);
    k_zeile_4.set_element(&klasse_3);
    k_zeile_4.set_element(&klasse_4);
    //k_zeile_4.set_element(&klasse_1);
    unt.AddKoppZeile(new CKoppZeile(k_zeile_4));

    CKoppZeile k_zeile_5;
    k_zeile_5.set_element(&lehrer_4);
    k_zeile_5.set_element(&klasse_3);
    k_zeile_5.set_element(&klasse_4);
    unt.AddKoppZeile(new CKoppZeile(k_zeile_5));

    return unt;
}

void test_polymorphism_25()
{
    std::cout << "*** start test_polymorphism_25 ***" << std::endl;

    CUnt unt_1{build_unterricht()};

    assert(unt_1.check_one_lehrer_pro_KZ() == true);
    assert(unt_1.check_one_lehrer_for_all_KZ() == true);
    assert(unt_1.if_one_klass_gleich_all_kalssen_gleich() == true);

    CUnt unt_2{ unt_1 };

    CUnt unt_3{};
    //unt_3 = unt_1;

    std::cout << "*** end test_polymorphism_25 ***" << std::endl;
}

/*
int main(int, char**)
{
    test_polymorphism_25();
    return 0;
}
*/