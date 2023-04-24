
//测试使用的类

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


	int* p;
};
