// ****************************************************************
//
//
//
//			THEDLL.HPP
//
//
//
// ****************************************************************



typedef void (*lpfnDllFuncType)(HDC, int, int);


class test_class
{
  private:
    static int x;
    static int y;
    
  public:
    test_class()
    {
      x = -1;
      y = -1;
    }

    ~test_class(){}

    static void FAR _export DllFunc(HDC hdc, int mx, int my);
};
