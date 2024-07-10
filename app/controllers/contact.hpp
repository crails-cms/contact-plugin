#pragma once
#include <crails/cms/controllers/injectable.hpp>
#include "app/controllers/application.hpp"
#include "../models/contact_page.hpp"

class ContactController : public ApplicationController
{
public:
  class Injectable : public Crails::Cms::Injectable
  {
    std::shared_ptr<ContactPage> page_settings;
  public:
    Injectable(const Crails::SharedVars&, Crails::RenderTarget&);
    bool require_page_settings();
    void run() override;
    const ContactPage* get_page_settings() const { return page_settings.get(); }
    void set_captcha_field(const std::string&);
    void set_error_message(const std::string&);
  };

  ContactController(Crails::Context&);

  void initialize() override;
  void finalize() override;

  void show();
  void submit();

  void submit_error(const std::string& message);
  void submit_mail(Data form);

  const ContactPage& get_page_settings() const { return *(injectable->get_page_settings()); }

private:
  std::unique_ptr<Injectable> injectable;
};
