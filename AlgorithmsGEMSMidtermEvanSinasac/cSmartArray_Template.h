#pragma once
// Templated Smart Array we did in class with Feeney

template <class T>
class cSmartArray_Template
{
public:
	cSmartArray_Template()
	{
		this->currentSize = INITIAL_SIZE;
		this->allocateSpace();
	};
	~cSmartArray_Template() {};

	T getAtIndex(unsigned int index)
	{
		T personToReturn = *(this->pData[index]);
		return personToReturn;
	}

	void setAtIndex(unsigned int index, T thePerson)
	{
		if (this->pData[index])
		{
			delete this->pData[index];
		}
		this->pData[index] = new T();
		*(this->pData[index]) = thePerson;
		return;
	}

	void allocateSpace(void)
	{
		unsigned int oldSize = this->currentSize;
		this->currentSize *= 2;
		T** pTempArray = new T * [this->currentSize];
		for (unsigned int index = 0; index != this->currentSize; index++)
		{
			//			pTempArray[index] = NULL;
			pTempArray[index] = nullptr;
		}
		for (unsigned int index = 0; index != this->currentEndIndex; index++)
		{
			pTempArray[index] = this->pData[index];
		}
		this->pData = pTempArray;
		return;
	}

	static const unsigned int INITIAL_SIZE = 5;
	unsigned int currentSize = 0;

	T** pData;	// allocated in constructor

	void insertAtEnd(T thePerson)
	{
		this->pData[this->currentEndIndex] = new T();
		*(this->pData[this->currentEndIndex]) = thePerson;
		this->currentEndIndex++;
		if (this->currentEndIndex > this->currentSize)
		{
			this->allocateSpace();
		}
		return;
	}

	unsigned int currentEndIndex = 0;

	unsigned int getSize(void)
	{
		return currentEndIndex;
	}

	// only thing I really added so I can remove things from the array if needed
	void removeAtIndex(unsigned int index)
	{
		if (index < this->currentEndIndex - 1)
		{
			for (unsigned int ind = index; ind != this->currentEndIndex - 1; ind++)
			{
				this->pData[ind] = this->pData[ind + 1];
			}

			//delete this->pData[this->currentEndIndex];
			//currentEndIndex -= 1;
			this->currentEndIndex--;
		}
		
		return;
	}

//	unsigned int getSize_by_counting_non_nulls(void)
//	{
//		unsigned int count = 0;
//		for (unsigned int index = 0; index != this->currentSize; index++)
//		{
//			// Is there a person there? (not NULL)
////			if (this->pData[index] != NULL)
//			if (this->pData[index] != nullptr)
//			{
//				count++;
//			}
//		}
//		return count;
//	}
};

