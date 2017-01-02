namespace ConstNamespace
{

class ConsterInterface
{
public:

virtual ~ConsterInterface()
{
}

virtual void constInvokedThroughNonConstMethod() const = 0;
virtual void constInvokedThroughNonConstMethod() = 0;
virtual void setValue(const int value ) const = 0;
virtual void setValue(const int value ) = 0;
virtual int getValue() const = 0;
virtual int getValue() = 0;
virtual void onlyNonOverloaded() const = 0;
virtual bool wasConstMethodUsed() const = 0;

};

}
