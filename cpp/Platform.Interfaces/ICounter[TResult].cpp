namespace Platform::Interfaces
{
    template <typename TResult> class ICounter<TResult>
    {
    public:
        virtual TResult Count() = 0;
    };
}
