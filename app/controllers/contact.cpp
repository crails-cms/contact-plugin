#include "contact.hpp"
#include "../mailer.hpp"
#include "lib/contact_page-odb.hxx"
#include <crails/params.hpp>
#include <crails/logger.hpp>
#include <crails/captcha/default.hpp>

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

void ContactController::Injectable::set_captcha_field(const string& html)
{
  vars["captcha_field"] = html;
}

void ContactController::Injectable::set_error_message(const string& message)
{
  vars["failure"] = true;
  vars["error_message"] = message;
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
  Crails::Captcha::Default captcha;

  injectable->set_captcha_field(captcha.render(params));
  injectable->run();
}

void ContactController::submit()
{
  auto self = shared_from_this();
  Data form = params["contact"];
  Crails::Captcha::Default captcha;

  if (form.require({"email", "message"}))
  {
    captcha.check(params, [this, self, form](bool success)
    {
      if (success)
      {
        submit_mail(form);
        render("contact/sent");
      }
      else
        submit_error(i18n::t("contact-page.captcha-invalid"));
    });
  }
  else
    submit_error(i18n::t("contact-page.form-invalid"));
}

void ContactController::submit_mail(Data form)
{
  auto mailer = std::make_shared<ContactMailer>(*this, form["email"].as<std::string>());

  vars["sender"] = form["email"].as<std::string>();
  vars["message"] = form["message"].as<std::string>();
  mailer->render("contact/mail");
  mailer->send([this, mailer]()
  {
    Crails::logger << Crails::Logger::Info << "ContactController::submit: mail sent" << Crails::Logger::endl;
  });
}

void ContactController::submit_error(const string& message)
{
  Crails::Captcha::Default captcha;

  injectable->set_captcha_field(captcha.render(params));
  injectable->set_error_message(i18n::t("contact-page.form-invalid"));
  injectable->run();
}
