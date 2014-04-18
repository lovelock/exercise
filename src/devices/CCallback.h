#ifndef CCallbackH
#define CCallbackH

/*
	cCallback 是允许子类定义一个回调函数的抽象类，例如在需要计算触觉力的时候由设备回调。这个功能并不是所有设备都支持
*/
class cCallback
{
	public:
	cCallback() {};

	virtual ~cCallback() {};

	virtual void callback() = 0;
};

#endif
