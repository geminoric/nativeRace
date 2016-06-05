#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class gameObject;

class component
{
public:
  gameObject *owner;

  virtual void onUpdate() {}
  virtual void onStart() {}
  virtual ~component() {}
  virtual const char *getCompName() { return "component"; }
};

#endif
