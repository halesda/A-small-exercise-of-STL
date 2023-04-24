

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

	test(const test& rhs) : p(new int(*rhs.p)) {}

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

	int* p;
};
