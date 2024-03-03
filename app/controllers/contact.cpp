#include "contact.hpp"
#include "../mailer.hpp"
#include "lib/contact_page-odb.hxx"
#include <crails/params.hpp>
#include <crails/logger.hpp>

using namespace std;

ContactController::Injectable::Injectable(const Crails::SharedVars& vars, Crails::RenderTarget& sink)
  : Crails::Cms::Injectable(vars, sink)
{
}

bool ContactController::Injectable::require_page_settings()
{
  return page_settings ? true : database.find_one(page_settings);
}

void ContactController::Injectable::run()
{
  if (require_page_settings())
  {
    render("contact/form", {
      {"contact_page", reinterpret_cast<const ContactPage*>(page_settings.get())}
    });
  }
}

ContactController::ContactController(Crails::Context& context) : ApplicationController(context)
{
}

void ContactController::initialize()
{
  ApplicationController::initialize();
  injectable = std::make_unique<Injectable>(vars, response);
  if (!injectable->require_page_settings())
    respond_with(Crails::HttpStatus::not_found);
  vars["contact_page"] = injectable->get_page_settings();
}

void ContactController::finalize()
{
  ApplicationController::finalize();
}

void ContactController::show()
{
  injectable->run();
}

void ContactController::submit()
{
  Data form = params["contact"];

  if (form.require({"email", "message"}))
  {
    auto mailer = std::make_shared<ContactMailer>(*this, form["email"].as<std::string>());

    vars["sender"] = form["email"].as<std::string>();
    vars["message"] = form["message"].as<std::string>();
    mailer->render("contact/mail");
    mailer->send([this, mailer]()
    {
      Crails::logger << Crails::Logger::Info << "ContactController::submit: mail sent" << Crails::Logger::endl;
    });
    render("contact/sent");
  }
  else
  {
    vars["failure"] = true;
    vars["error_message"] = i18n::t("contact-page.form-invalid");
    injectable->run();
  }
}

void ContactController::submit_error()
{
  vars["failure"] = true;
  vars["error_message"] = i18n::t("contact-page.submit-error");
  injectable->run();
}
