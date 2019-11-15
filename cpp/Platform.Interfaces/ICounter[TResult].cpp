namespace Platform::Interfaces
{
    template <typename TResult> class ICounter
    {
    public:
        virtual TResult Count() = 0;
    };
}
