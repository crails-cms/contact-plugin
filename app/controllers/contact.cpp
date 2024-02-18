#include "contact.hpp"
#include "../mailer.hpp"
#include "lib/contact_page-odb.hxx"
#include <crails/params.hpp>
#include <crails/logger.hpp>

using namespace std;

ContactController::ContactController(Crails::Context& context) : ApplicationController(context)
{
}

void ContactController::initialize()
{
  ApplicationController::initialize();
  if (!database.find_one(page_settings))
    respond_with(Crails::HttpStatus::not_found);
  vars["contact_page"] = reinterpret_cast<const ContactPage*>(page_settings.get());
}

void ContactController::finalize()
{
  ApplicationController::finalize();
}

void ContactController::show()
{
  render("contact/form");
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
    render("contact/form");
  }
}

void ContactController::submit_error()
{
  vars["failure"] = true;
  vars["error_message"] = i18n::t("contact-page.submit-error");
  render("contact/form");
}
