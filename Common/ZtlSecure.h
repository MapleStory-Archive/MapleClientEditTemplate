#pragma once
#include <Windows.h>
#include <condition_variable>

/*
	Credits: https://github.com/67-6f-64/Firefly/blob/master/Firefly%20Spy/ZtlSecure.hpp

	Modifications By:
		 - Rajan Grewal
		 - Minimum Delta
*/

#define ZTLSECURE_CHECKSUM 0xBAADF00D
#define ZTLSECURE_ROTATION 5

// todo
class SECRECT
{
private:
	int _ZtlSecureTear_left[2];
	unsigned int _ZtlSecureTear_left_CS;
	int _ZtlSecureTear_top[2];
	unsigned int _ZtlSecureTear_top_CS;
	int _ZtlSecureTear_right[2];
	unsigned int _ZtlSecureTear_right_CS;
	int _ZtlSecureTear_bottom[2];
	unsigned int _ZtlSecureTear_bottom_CS;

public:
	void SetRectEmpty()
	{

	}

	int GetRight() // original name: ZtlSecureGet_right
	{

	}

	int GetLeft() // original name: ZtlSecureGet_left
	{

	}

	void PutRight()
	{

	}

	void PutLeft()
	{

	}
};

template <typename T> // uses fastcall because it passes args through registers instead of on stack -- faster execution
T __fastcall ZtlSecureTear(T* at, T t)
{
	/* storage type is 1 byte large if size < 4, otherwise its 4 bytes large */
	typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type PType;

	unsigned int checksum = ZTLSECURE_CHECKSUM;

	PType* key = reinterpret_cast<PType*>(&at[0]);
	PType* encrypted_data = reinterpret_cast<PType*>(&at[1]);

	PType* p = reinterpret_cast<PType*>(&t);

	for (int i = 0; i < sizeof(T) / sizeof(PType); i++)
	{
		int rotations = sizeof(T) < sizeof(unsigned int) ? NULL : ZTLSECURE_ROTATION;

		/* TODO use CRand32 like MapleStory does */
		key[i] = sizeof(T) < sizeof(unsigned int) ? LOBYTE(rand()) : rand();
		encrypted_data[i] = _rotr(p[i] ^ key[i], rotations);

		checksum = encrypted_data[i] + _rotr(key[i] ^ checksum, ZTLSECURE_ROTATION);
	}

	return checksum;
}

template <typename T>
T __fastcall ZtlSecureFuse(T* at, unsigned int uCS)
{
	/* storage type is 1 byte large if size < 4, otherwise its 4 bytes large */
	typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type PType;

	unsigned int checksum = ZTLSECURE_CHECKSUM;

	PType* key = reinterpret_cast<PType*>(&at[0]);
	PType* encrypted_data = reinterpret_cast<PType*>(&at[1]);

	PType value[sizeof(T) / sizeof(PType)] = { 0 };

	for (int i = 0; i < sizeof(T) / sizeof(PType); i++)
	{
		int rotations = sizeof(T) < sizeof(unsigned int) ? NULL : ZTLSECURE_ROTATION;

		value[i] = key[i] ^ _rotl(encrypted_data[i], rotations);
		checksum = encrypted_data[i] + _rotr(key[i] ^ checksum, ZTLSECURE_ROTATION);
	}

	if (checksum != uCS)
	{
		// TODO exception handling
		/*ZException zException = { 5 };
		CxxThrowException(&zException, (void*)ThrowInfo::ZException);*/
	}

	return  *reinterpret_cast<T*>(&value[0]);
}