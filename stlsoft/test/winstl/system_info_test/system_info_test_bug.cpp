

    void get_bool(bool *pb)
    {
        *pb = false;
    }

    void fn()
    {
        static bool s_variable;
#if 1
        static bool s_init = (get_bool(&s_variable), true);
#else
        static bool s_init = false;

        if(!s_init)
        {
            get_bool(&s_variable);
            s_init = true;
        }
#endif /* 0 */
    }

    int main(int /* argc */, char ** /*argv*/)
    {
        fn();
        fn();

        return 0;
    }
