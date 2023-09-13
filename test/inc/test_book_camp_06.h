#pragma once

#include <memory>
#include <vector>

namespace Race
{
	class Blob
	{

	public:
		virtual ~Blob() = default;
		Blob() = default;

		Blob(Blob const&) = delete;
		Blob& operator=(Blob const&) = delete;

		virtual void step() = 0;
		virtual int total_steps() const = 0;
	};

	class StepperBlob : public Blob
	{
		int y = 0;

	public:
		void step() override
		{
			y += 2;
		}

		int total_steps() const override
		{
			return y;
		}
	};

	void move_blobs(std::vector<StepperBlob>& blobs);
	void draw_blobs(const std::vector<StepperBlob>& blobs);
	void race(std::vector<StepperBlob>& blobs);

	template <typename T, typename U>
	class RandomBlob : public Blob
	{
		int y = 0;
		T generator;
		U distribution;

	public:
		RandomBlob(T gen, U dis)
			: generator(gen)
			, distribution(dis)
		{}

		void step() override
		{
			y += static_cast<int>(distribution(generator));
		}

		int total_steps() const override
		{
			return y;
		}
	};

	void race(std::vector<std::unique_ptr<Blob>>& blob);
	void move_blobs(const std::vector<std::unique_ptr<Blob>>& blobs);
	void draw_blobs(const std::vector<std::unique_ptr<Blob>>& blob);
}

void test_boot_camp_polymorphism_069();
void test_boot_camp_polymorphism_069_01();
