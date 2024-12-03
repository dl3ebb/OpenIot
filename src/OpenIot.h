#ifndef IOT_CORE_H
#define IOT_CORE_H
#include <Modules/Module.h>
#include <Logger/Logger.h>
#include <Connector/Connector.h>
#include <WebWidgets/WebForm.h>
#include <vector>

class OpenIot {
   private:
      std::vector<Connector*> connectors;
      std::vector<Module*> modules;
      std::vector<WebForm*> forms;

      bool firstLoop = true;

      String applicationName;
      int applicationBuild;


   public : 
      OpenIot ();
      void registerConnector (Connector* connector);
      void registerModule (Module* module);
      void registerForm (WebForm* form);
      void setLogger (Logger* logger_);
      void setup (String applicationName, int build);
      void loop ();

      String getApplicationName () {return applicationName; }
      int getApplicationBuild () { return applicationBuild; }

      void rebootDevice ();

};

extern OpenIot openIot;

#endif