#ifndef INDEX_PAGE_H
#define INDEX_PAGE_H
#include <Connector/WebConnector.h>

/**
 * Indexpage is the form that holds the rebootCountdown and the rebootNeededLabel.
 */
class IndexPage : public WebForm {

  private:
      WebStringElement*     rebootRequired;
      WebCountdownElement*  rebootCountdown; 

  public:
    IndexPage();
    bool checkFormData() override { return true; };
    void submit() override {};
    void setup() override {};
};


#define indexPage getIndexPage()

IndexPage *getIndexPage();

#endif