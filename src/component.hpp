#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class component
{
public:
  virtual void onUpdate() {}
  virtual void onStart() {}
  virtual ~component() {}
  virtual const char *getCompName() { return "component"; }
  //The id of the component, each component type has a unique id
  static int totalID;
  static int id;
};

#endif
