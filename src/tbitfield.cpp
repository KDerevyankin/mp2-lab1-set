// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
	{
		throw "Negative lenght";
	}
	BitLen = len;
	MemLen = (((sizeof(TELEM) * 8) - 1 + BitLen) / (sizeof(TELEM) * 8));
	pMem = new TELEM[MemLen];
	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (sizeof(TELEM) * 8) - 1);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
	{
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	}
	else
	{
		throw ("Large index");
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen)
	{
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	}
	else
	{
		throw ("Large index");
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < BitLen)
	{
		if (pMem[GetMemIndex(n)] & GetMemMask(n))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		throw ("Large index");
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[]pMem;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
	return (*this);
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 0;
	}
	for (size_t i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	unsigned int MaxLen;
	if (BitLen > bf.BitLen)
	{
		MaxLen = BitLen;
	}
	else
	{
		MaxLen = bf.BitLen;
	}
	TBitField res(MaxLen);
	for (size_t i = 0; i < MemLen; i++)
	{
		res.pMem[i] = pMem[i];
	}
	for (size_t i = 0; i < bf.MemLen; i++)
	{
		res.pMem[i] |= bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	unsigned int n, m;
	if (BitLen >= bf.BitLen)
	{
		n = BitLen;
	}
	else
	{
		n = bf.BitLen;
	}
	if (BitLen >= bf.BitLen)
	{
		m = bf.BitLen;
	}
	else
	{
		m = BitLen;
	}
	TBitField res(n);
	for (size_t i = 0; i < m; i++)
	{
		if (GetBit(i) & bf.GetBit(i))
		{
			res.SetBit(i);
		}
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res = (*this);
	for (size_t i = 0; i < BitLen; i++)
	{
		if (res.GetBit(i))
		{
			res.ClrBit(i);
		}
		else
		{
			res.SetBit(i);
		}
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	unsigned int tmp;
	for (size_t i = 0; i < bf.BitLen; i++)
	{
		istr >> tmp;
		if (tmp)
		{
			bf.SetBit(tmp);
		}
		else
		{
			bf.ClrBit(tmp);
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (size_t i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
