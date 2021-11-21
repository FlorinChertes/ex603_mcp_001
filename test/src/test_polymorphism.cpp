#include <map>
#include <vector>
#include <string>
#include <iostream>

#include <memory>
#include <cassert>
#include <algorithm>

namespace unterricht
{
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    class CStpEl
    {
    public:
        void set_name(const std::string& name) { m_name = name; }
        const std::string& get_name() const noexcept { return m_name; }

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
        [[nodiscard]] bool set_element(CStpEl* elem) {
            bool is_unique_inserted = false;
            auto it = std::ranges::find_if(m_arStpEl.begin(), m_arStpEl.end(),
                [elem](const auto item) {
                    return item->get_name() == elem->get_name();
                }
            );

            if (it == m_arStpEl.end()) {
                m_arStpEl.push_back(elem);
                is_unique_inserted = true;
            }
            return is_unique_inserted;
        }

        bool check_one_lehrer_pro_KZ() const;
        CLe* get_lehrer() const;
        bool check_all_klassen_gleich(const CKoppZeile& other) const;
        std::vector<std::string> get_all_klassen_name() const;

    private:
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
        return std::ranges::count_if(m_arStpEl.begin(), m_arStpEl.end(), CastableTo<CLe>) == 1;
    }

    CLe* CKoppZeile::get_lehrer() const {
        auto it = std::ranges::find_if(m_arStpEl.begin(), m_arStpEl.end(), CastableTo<CLe>);
        return it != m_arStpEl.end() ? static_cast<CLe*>(*it) : nullptr;
    }

    bool CKoppZeile::check_all_klassen_gleich(const CKoppZeile& other) const
    {
        bool all_kalssen_gleich = true;

        if (m_arStpEl.size() != other.m_arStpEl.size()) {
            all_kalssen_gleich = false;
        }

        if (all_kalssen_gleich == true) {
            for (const auto& elem : m_arStpEl) {

                assert(elem != nullptr);
                const auto& klasse = dynamic_cast<CKla*>(elem);

                if (klasse != nullptr) {
                    bool found = false;
                    for (auto& curr_klasse : other.m_arStpEl) {
                        if (curr_klasse->get_name() == klasse->get_name()) {
                            found = true;
                            break;
                        }
                    }

                    if (found == false) {
                        all_kalssen_gleich = false;
                    }
                }

                if (all_kalssen_gleich == false) {
                    break;
                }
            }
        }
        return all_kalssen_gleich;
    }


    std::vector<std::string> CKoppZeile::get_all_klassen_name() const {

        std::vector<std::string> klassen_names;

        for (const auto& elem : m_arStpEl) {
            assert(elem != nullptr);
            const auto& stp_elem = dynamic_cast<CKla*>(elem);
            if (stp_elem != nullptr) {
                klassen_names.push_back(stp_elem->get_name());
            }
        }
        assert(klassen_names.empty() == false);
        return klassen_names;
    }


    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

    class CUnt
    {
        friend void swap(CUnt& lhs, CUnt& rhs) {
            std::swap(lhs.m_arKoppZeile, rhs.m_arKoppZeile);
        }


    public:
        CUnt() = default;

        CUnt(const CUnt& unt)
        {
            for (CKoppZeile* pKoppZeile : unt.m_arKoppZeile)
            {
                m_arKoppZeile.push_back(new CKoppZeile(*pKoppZeile));
            }
        }

        CUnt& operator= (const CUnt& rhs)
        {
            if (this != &rhs)
            {
                CUnt temp{ rhs };
                swap(*this, temp);
            }
            return *this;
        }

        ~CUnt()
        {
            for (CKoppZeile* pKoppZeile : m_arKoppZeile)
            {
                delete pKoppZeile;
            }
        }

        void AddKoppZeile(CKoppZeile* pKoppZeile)
        {
            m_arKoppZeile.push_back(pKoppZeile);
        }

        bool check_one_lehrer_pro_KZ() const;
        bool check_one_lehrer_for_all_KZ() const;
        bool if_one_klass_gleich_all_kalssen_gleich() const;

    private:

        std::vector<CKoppZeile*> m_arKoppZeile;
    };


    bool CUnt::check_one_lehrer_pro_KZ() const {
        bool is_valid = true;
        for (const auto& elem : m_arKoppZeile) {
            assert(elem != nullptr);
            if (elem->check_one_lehrer_pro_KZ() == false) {
                is_valid = false;
            }
        }
        return is_valid;
    }

    bool CUnt::check_one_lehrer_for_all_KZ() const {

        assert(check_one_lehrer_pro_KZ() == true);

        std::vector<std::string> lehrer_array;
        bool is_valid = true;

        for (const auto& elem : m_arKoppZeile) {
            assert(elem != nullptr);

            const CLe* lehrer = elem->get_lehrer();
            assert(lehrer != nullptr);

            auto it = std::ranges::find(lehrer_array.begin(), lehrer_array.end(), lehrer->get_name());
            if (it == lehrer_array.end()) {
                lehrer_array.push_back(lehrer->get_name());
            }
            else {
                is_valid = false;
                break;
            }

        }

        return is_valid;
    }

    bool CUnt::if_one_klass_gleich_all_kalssen_gleich() const {
        assert(check_one_lehrer_pro_KZ() == true);

        std::map<std::string, const CKoppZeile*, std::less<>> klassen_map;
        bool is_valid = true;

        for (const auto& elem : m_arKoppZeile) {
            assert(elem != nullptr);

            const std::vector<std::string> klasse_names{ elem->get_all_klassen_name() };

            for (const auto& klass_name : klasse_names) {
                auto it = klassen_map.find(klass_name);
                if (it == klassen_map.end()) {
                    klassen_map[klass_name] = elem;
                }
                else {
                    is_valid = elem->check_all_klassen_gleich(*it->second);
                    break;
                }
            }

            if (is_valid == false) {
                break;
            }
        }

        return is_valid;
    }



unterricht::CLe lehrer_1("L1_a");
unterricht::CLe lehrer_2("L2_a");
unterricht::CLe lehrer_3("L3_a");
unterricht::CLe lehrer_4("L4_a");

unterricht::CKla klasse_1("K1_a");
unterricht::CKla klasse_2("K2_a");

unterricht::CKla klasse_3("K3_a");
unterricht::CKla klasse_4("K4_a");
unterricht::CKla klasse_5("K5_a");
unterricht::CKla klasse_6("K6_a");

unterricht::CUnt build_unterricht()
{
    bool is_unique_inserted{ false };

    unterricht::CKoppZeile k_zeile_1;
    is_unique_inserted = k_zeile_1.set_element(&lehrer_1);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_1.set_element(&klasse_1);
    assert(is_unique_inserted == true);

    //      1. in jeder Kopplungszeile höchstens 1 Lehrer vorkommt
    //k_zeile_1.set_element(&lehrer_3);

    unterricht::CKoppZeile k_zeile_2;
    is_unique_inserted = k_zeile_2.set_element(&lehrer_2);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_2.set_element(&klasse_2);
    assert(is_unique_inserted == true);

    unterricht::CUnt unt;

    unt.AddKoppZeile(new unterricht::CKoppZeile(k_zeile_1));
    unt.AddKoppZeile(new unterricht::CKoppZeile(k_zeile_2));

    //      2. jeder Lehrer im Unterricht höchstens 1-mal vorkommt und
    /*
    CKoppZeile k_zeile_3;
    k_zeile_3.set_element(&lehrer_2);
    k_zeile_3.set_element(&klasse_3);
    unt.AddKoppZeile(new CKoppZeile(k_zeile_3));
    //*/

    //      3. wenn zwei Kopplungszeilen eine Klasse gemeinsam haben, dann
    //              sind alle Klassen gleich.
    unterricht::CKoppZeile k_zeile_4;
    is_unique_inserted = k_zeile_4.set_element(&lehrer_3);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_4.set_element(&klasse_3);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_4.set_element(&klasse_4);
    assert(is_unique_inserted == true);

    //is_unique_inserted = k_zeile_4.set_element(&klasse_1);
    //sassert(is_unique_inserted == true);

    unt.AddKoppZeile(new unterricht::CKoppZeile(k_zeile_4));

    unterricht::CKoppZeile k_zeile_5;
    is_unique_inserted = k_zeile_5.set_element(&lehrer_4);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_5.set_element(&klasse_3);
    assert(is_unique_inserted == true);
    is_unique_inserted = k_zeile_5.set_element(&klasse_4);
    assert(is_unique_inserted == true);
    unt.AddKoppZeile(new unterricht::CKoppZeile(k_zeile_5));

    return unt;
}

} // namespace unterricht

void test_polymorphism_25_ante()
{
    std::cout << "*** start test_polymorphism_25 ante ***" << std::endl;

    unterricht::CUnt unt_1{unterricht::build_unterricht()};

    assert(unt_1.check_one_lehrer_pro_KZ() == true);
    assert(unt_1.check_one_lehrer_for_all_KZ() == true);
    assert(unt_1.if_one_klass_gleich_all_kalssen_gleich() == true);

    //unterricht::CUnt unt_2{ unt_1 };

    //unterricht::CUnt unt_3{};
    //unt_3 = unt_1;

    std::cout << "*** end test_polymorphism_25 ante ***" << std::endl;
}
