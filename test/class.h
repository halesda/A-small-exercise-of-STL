
//测试使用的类
//
using std::istream,std::ostream,std::cin,std::cout;

struct test
{
	test(int a  = 0) : p(new int(a)) {}

	~test() noexcept
	{
		if(p)
		{
			delete p;
		}
		p = 0;
	}

	test(const test& rhs) noexcept 
	{
		if(rhs.p)
		{
			p = new int(*rhs.p);
		}
		else
		{
			p = new int(0);
		}
	}

	test& operator=(const test& rhs) noexcept
	{
		if(this != &rhs)
		{
			if(p)
			{
				delete p;
			}
			if(rhs.p)
			{
				p = new int(*rhs.p);
			}
			else
			{
				p = new int(0);
			}
		}
		return *this;
	}


	bool operator ==(const test& rhs)const noexcept
	{
		return *p == *rhs.p;
	}

	bool operator !=(const test& rhs)const noexcept
	{
		return !(*this == rhs);
	}

	bool operator > (const test& rhs) const noexcept
	{
		return *p > *rhs.p;
	}

	bool operator < (const test& rhs) const noexcept
	{
		return *p < *rhs.p;
	}

	friend ostream& operator <<(ostream& os,const test& rhs) noexcept
	{
		return os << *rhs.p;
	}

	friend istream& operator >>(istream& is,test& rhs) noexcept
	{
		int a = 0;
		cin >> a;
		if(rhs.p)
		{
			*rhs.p = a;
		}
		else
		{
			rhs.p = new int(a);
		}
		return is;
	}


	int* p;
};
