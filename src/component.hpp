#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class component
{
public:
  virtual void onUpdate() {}
  virtual void onStart() {}
  virtual ~component() {}
  //The id of the component, each component type has a unique id
  static int totalID;
  int id = 0;
};

#endif
