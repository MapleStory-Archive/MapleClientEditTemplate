#pragma once
#include <Windows.h>

/*
	Original Credits: https://github.com/67-6f-64/Firefly/blob/master/Firefly%20Spy/TSecType.hpp
	Modifications Made By:
		- Rajan Grewal
		- Minimum Delta
*/

template <typename T>
struct TSecData
{
	T data;
	UCHAR bKey;
	UCHAR FakePtr1;
	UCHAR FakePtr2;
	USHORT wChecksum;
};

template <typename T>
class TSecType
{
private:
	UINT FakePtr1;
	UINT FakePtr2;
	TSecData<T>* m_secdata;

public:
	TSecType()
	{
		this->m_secdata = new TSecData<T>(); // uses proper ZAllocEx now (since global new operator overload)

		this->FakePtr1 = static_cast<UINT>(rand());
		this->FakePtr2 = static_cast<UINT>(rand());

		this->m_secdata->FakePtr1 = static_cast<UCHAR>(LOBYTE(this->FakePtr1));
		this->m_secdata->FakePtr2 = static_cast<UCHAR>(LOBYTE(this->FakePtr2));

		this->SetData(NULL);
	}

	TSecType(const T op)
	{
		this->m_secdata = new TSecData<T>(); // uses proper ZAllocEx now (since global new operator overload)

		this->FakePtr1 = static_cast<UINT>(rand());
		this->FakePtr2 = static_cast<UINT>(rand());

		this->m_secdata->FakePtr1 = static_cast<UCHAR>(LOBYTE(this->FakePtr1));
		this->m_secdata->FakePtr2 = static_cast<UCHAR>(LOBYTE(this->FakePtr2));

		this->SetData(op);
	}

	~TSecType()
	{
		if (this->m_secdata)
		{
			delete this->m_secdata;
		}
	}

	operator T()
	{
		return this->GetData();
	}

	BOOL operator ==(TSecType<T>* op)
	{
		return this->GetData() == op->GetData();
	}

	TSecType<T> operator =(const T op)
	{
		this->SetData(op);
		return this;
	}

	TSecType<T> operator =(TSecType<T>* op)
	{
		T data = op->GetData();
		this->SetData(data);
		return this;
	}

	T operator /=(const T op)
	{
		T tmp = this->GetData() / op;
		this->SetData(tmp);
		return tmp;
	}

	T operator *=(const T op)
	{
		T tmp = this->GetData() * op;
		this->SetData(tmp);
		return tmp;
	}

	T operator +=(const T op)
	{
		T tmp = this->GetData() + op;
		this->SetData(tmp);
		return tmp;
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