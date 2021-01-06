#pragma once
#include <Windows.h>

// thanks raj

template <typename T>
struct TSecData
{
	T data;
	unsigned char bKey;
	unsigned char FakePtr1;
	unsigned char FakePtr2;
	unsigned short wChecksum;
};

template <typename T>
class TSecType
{
public:
	TSecType()
	{
		this->m_secdata = reinterpret_cast<TSecData<T>*>(malloc(12));

		this->FakePtr1 = static_cast<unsigned int>(reinterpret_cast<unsigned int>(&this[-0x00003FF8]) + rand());
		this->FakePtr2 = static_cast<unsigned int>(reinterpret_cast<unsigned int>(&this[-0x00003FF8]) + rand());

		this->m_secdata->FakePtr1 = static_cast<unsigned char>(LOBYTE(this->FakePtr1));
		this->m_secdata->FakePtr2 = static_cast<unsigned char>(LOBYTE(this->FakePtr2));

		this->SetData(0);
	}

	~TSecType()
	{
		if (this->m_secdata)
		{
			free(this->m_secdata);
			this->m_secdata = nullptr;
		}
	}

	T GetData()
	{
		T decrypted_data = 0;
		unsigned short checksum = 0;

		for (unsigned char i = 0, key = 0; i < (sizeof(T) + 1); i++)
		{
			key = (i == 0 ? this->m_secdata->bKey : reinterpret_cast<unsigned char*>(&this->m_secdata->data)[i - 1] + key + 42);

			if (i > 0)
			{
				checksum = (i > 1 ? ((8 * checksum) | (key + (checksum >> 13))) : ((key + 4) | 0xD328));
			}

			if (i < sizeof(T))
			{
				if (!key)
				{
					key = 42;
				}

				reinterpret_cast<unsigned char*>(&decrypted_data)[i] = reinterpret_cast<unsigned char*>(&this->m_secdata->data)[i] ^ key;
			}
		}

		if (this->m_secdata->wChecksum != checksum || LOBYTE(this->FakePtr1) != this->m_secdata->FakePtr1 || LOBYTE(this->FakePtr2) != this->m_secdata->FakePtr2)
		{
			return 0; //TODO: CxxThrow
		}

		return decrypted_data;
	}

	void SetData(T data)
	{
		this->m_secdata->bKey = static_cast<unsigned char>(rand());
		this->m_secdata->wChecksum = static_cast<unsigned short>(39525);

		for (unsigned char i = 0, key = 0; i < (sizeof(T) + 1); i++)
		{
			key = (i == 0 ? this->m_secdata->bKey : (key ^ reinterpret_cast<unsigned char*>(&data)[i - 1]) + key + 42);

			if (i > 0)
			{
				this->m_secdata->wChecksum = ((8 * this->m_secdata->wChecksum) | (key + (this->m_secdata->wChecksum >> 13)));
			}

			if (i < sizeof(T))
			{
				if (!key)
				{
					key = 42;
				}

				reinterpret_cast<unsigned char*>(&this->m_secdata->data)[i] = reinterpret_cast<unsigned char*>(&data)[i] ^ key;
			}
		}
	}

private:
	unsigned int FakePtr1;
	unsigned int FakePtr2;
	TSecData<T>* m_secdata;
};

struct SECPOINT
{
	TSecType<long> y;
	TSecType<long> x;

	operator POINT()
	{
		return { this->x.GetData(), this->y.GetData() };
	}
};

struct SECRECT
{
	int _ZtlSecureTear_left[2];
	unsigned int _ZtlSecureTear_left_CS;
	int _ZtlSecureTear_top[2];
	unsigned int _ZtlSecureTear_top_CS;
	int _ZtlSecureTear_right[2];
	unsigned int _ZtlSecureTear_right_CS;
	int _ZtlSecureTear_bottom[2];
	unsigned int _ZtlSecureTear_bottom_CS;
};