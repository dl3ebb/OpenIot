#ifndef MODULE_H
#define MODULE_H
/**
 * A Module is a pieco of code that adds functionality to the application. (NTP, Update ...)
 * It may be used by registering the module to the opeiIot core
 */
class Module {
  public:
    virtual void __setup();
    virtual void setup() = 0;
    virtual void loop(bool firstLoop) = 0;
    virtual ~Module() = default;
    void depend() {}
};

#endif