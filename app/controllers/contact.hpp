#pragma once
#include "app/controllers/application.hpp"
#include "../models/contact_page.hpp"

class ContactController : public ApplicationController
{
public:
  ContactController(Crails::Context&);

  void initialize() override;
  void finalize() override;

  void show();
  void submit();

  void submit_error();

  const ContactPage& get_page_settings() const { return *page_settings; }

private:
  std::shared_ptr<ContactPage> page_settings;
};
