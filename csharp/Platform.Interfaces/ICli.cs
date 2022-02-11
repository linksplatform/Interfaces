namespace Platform.Interfaces;

public interface ICli<TLinkAddress>
{
    void Run(params string[] args);
}
