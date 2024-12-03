#ifndef WEB_UPDATE_FORM_H
#define WEB_UPDATE_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebProgressDialogElement.h>
#include <WebWidgets/WebStringElement.h>

class WebUpdateForm : public WebForm {

  private:
    WebBoolElement *enableUpdate;
    WebStringElement *server;
    WebBoolElement *autoUpdate;
    WebButtonElement *save;
    WebStringElement *software;
    WebStringElement *installedVersion;
    WebStringElement *availableVersion;
    WebButtonElement *doUpdateButton;
    WebProgressDialogElement *progressDialog;

  public:
    WebUpdateForm();
    bool checkFormData() override;
    void submit() override;
    void setup() override;
};

#define webUpdateForm getWebUpdateForm()

WebUpdateForm *getWebUpdateForm();

#endif
