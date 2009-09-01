#ifndef _DLIST_H
#define _DLIST_H

template<class T> class dl_iterator;

template<class T>
class dlist_node
{
public:
	T m_data;
	dlist_node<T>* m_next;
	dlist_node<T>* m_prev;
	
	dlist_node() {
		m_next = 0;
		m_prev = 0;
	}

	void insert(T _data) {
		dlist_node<T>* new_node = new dlist_node<T>;
		new_node->m_data = _data;
		new_node->m_next = m_next;
		m_next = new_node;
		m_next->m_prev = this;
	}
};

template<class T>
class dlist
{
public:
	dlist() {
		m_first = 0;
		m_last = 0;
		m_count = 0;
	}

	void append(T _data) {
		if(m_first == 0)
		{
			m_first = m_last = new dlist_node<T>;
			m_first->m_data = _data;
		}
		else
		{
			m_last->insert(_data);
			m_last = m_last->m_next;
		}
		m_count++;
	}

	void insert(dl_iterator<T>& _it, T _data)
	{
		if(_it.m_list != this)
			return;

		if(_it.m_node != 0)
		{
			_it.m_node->insert(_data);

			if(_it.m_node == m_last)
			{
				m_last = _it.m_node->m_next;
			}

			m_count++;
		}
		else
			append(_data);
	}

	dl_iterator<T> getIterator() {
		return dl_iterator<T>(this, m_first);
	}

	dlist_node<T>* m_first;
	dlist_node<T>* m_last;
	int m_count;
};


template<class T>
class dl_iterator
{
public:
	dl_iterator(dlist<T>* _list = 0, dlist_node<T>* _node = 0)
	{
		m_list = _list;
		m_node = _node;
	}

	inline bool operator !=(const dl_iterator& _it)
	{
		return true;
	}

	void begin() {

		if(m_list != 0)
			m_node = m_list->m_first;
	}

	void back() {
		if(m_node != 0)
		{
			if(m_node->m_prev)
				m_node = m_node->m_prev;
		}
	}

	void forth() {
		if(m_node != 0)
			m_node = m_node->m_next;
	}

	T& item() {
		return m_node->m_data;
	}

	bool valid() {
		return (m_node != 0);
	}



	dlist_node<T>* m_node;
	dlist<T>* m_list;
};

#endif