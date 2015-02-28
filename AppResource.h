#ifndef _cappresource_h
#define _cappresource_h

#include "base.h"

template<class T> class CAppResource
{	
	typedef std::map<std::string, T*> RESOURCEMAP;
	
	typename RESOURCEMAP m_ResourceMap;
	typename RESOURCEMAP::iterator m_ResourceIter;
	std::pair<typename RESOURCEMAP::iterator, BOOL> m_CurrentIter;
	T* m_pCurrent;

public:
	CAppResource();
	~CAppResource();
	
	T* GetResource(int index);
	T* GetCurrent() const;
	T* GetResource(const char *pTag);
	void AddResource(const char *pTag);
	void SetCurrent(const char *pTag);
};

template <class T> CAppResource<T>::CAppResource()
{
	m_pCurrent = NULL;
}

template <class T> CAppResource<T>::~CAppResource()
{
	for(m_ResourceIter = m_ResourceMap.begin(); m_ResourceIter != m_ResourceMap.end(); ++m_ResourceIter)
		CBase::SafeDelete((*m_ResourceIter).second, "Releasing Resource");		
}

template <class T> T* CAppResource<T>::GetCurrent() const
{
	assert(m_pCurrent);
	return m_pCurrent;
}

template <class T> T* CAppResource<T>::GetResource(int index)
{
	m_ResourceIter = m_ResourceMap.begin();

	while(index--)
	{
		m_ResourceIter++;
	}

	return (*m_ResourceIter).second;
}

template <class T> T* CAppResource<T>::GetResource(const char *pTag)
{
	m_ResourceIter = m_ResourceMap.find(pTag);

	assert(m_ResourceIter != m_ResourceMap.end());	
	assert((*m_ResourceIter).second);
	
	return (*m_ResourceIter).second;
}

template<class T> void CAppResource<T>::AddResource(const char *pTag)
{
	m_CurrentIter = m_ResourceMap.insert(RESOURCEMAP::value_type(pTag, new T));
	//m_ResourceIter = m_CurrentIter.first;
	//m_pCurrent = (*m_ResourceIter).second;
	if(m_CurrentIter.second)
		m_pCurrent = (*m_CurrentIter.first).second;
}

template <class T> void CAppResource<T>::SetCurrent(const char *pTag)
{
	m_ResourceIter = m_ResourceMap.find(pTag);
	assert(m_ResourceIter != m_ResourceMap.end());

	m_pCurrent = (*m_ResourceIter).second;
	assert(m_pCurrent);
}

#endif


	
