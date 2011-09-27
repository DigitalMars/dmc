
// Updated: 7th February 2003

#include <stlsoft.h>


class MutableTest
{
// Construction
public:
	MutableTest()
		: m_value(0)
		, m_cAccesses(0)
	{}

// Attributes
public:
	int value() const
	{
		++stlsoft_ns_qual(mutable_access)(m_cAccesses);

		return m_value;
	}

// Members
protected:
	int					m_value;
	ss_mutable_k int	m_cAccesses;
};

int main(int /* argc */, char /*argv*/[])
{
	MutableTest	mt;

	mt.value();

	return 0;
}
