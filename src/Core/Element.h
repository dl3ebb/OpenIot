#ifndef ELEMENT_H
#define ELEMENT_H
#include "EventManager.h"
#include <Arduino.h>

/**
 * An Element is a holder for a value. OpenIot knows 4 different types of Elements listed below.
 *
 * An Element may have differnt representations in the application
 */

enum ElementType {
    VOID_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    LONG_TYPE,
};

class Element {
  public:
    String id;
    Element(String id_) : id(id_) {}
    virtual ElementType getType() = 0;
};

class ButtonElement : public Element {
  public:
    ButtonElement(String id_) : Element(id_) {}
    ElementType getType() override { return VOID_TYPE; }
};

class ProgressDialogElement : public Element {
  public:
    ProgressDialogElement(String id_) : Element(id_) {}
    ElementType getType() override { return VOID_TYPE; }
};

class CountdownElement : public Element {
  public:
    CountdownElement(String id_) : Element(id_) {}
    ElementType getType() override { return VOID_TYPE; }
};

class SelectElement : public Element {
  public:
    SelectElement(String id_) : Element(id_) {}
    ElementType getType() override { return STRING_TYPE; }
    void setValue(String value_) {
        eventManager.fireEvent(new NewStringValueEvent(id, value_));
    }
};

class BoolElement : public Element {
  public:
    BoolElement(String id_) : Element(id_) {}
    ElementType getType() override { return BOOL_TYPE; }
    void setValue(bool value_) {
        eventManager.fireEvent(new NewBoolValueEvent(id, value_));
    }
};

class StringElement : public Element {
  public:
    StringElement(String id_) : Element(id_) {}
    ElementType getType() override { return STRING_TYPE; }
    void setValue(String value_) {
        eventManager.fireEvent(new NewStringValueEvent(id, value_));
    }
};

class LongElement : public Element {
  public:
    LongElement(String id_) : Element(id_) {}
    ElementType getType() override { return LONG_TYPE; }
    void setValue(long value_) {
        eventManager.fireEvent(new NewLongValueEvent(id, value_));
    }
};

#endif