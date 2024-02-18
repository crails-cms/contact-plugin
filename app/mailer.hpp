#pragma once
#include <crails/mailer.hpp>
#include "models/contact_page.hpp"

class ContactMailer : public Crails::Mailer
{
public:
  ContactMailer(ContactController& controller, const std::string& reply_to) : Crails::Mailer(controller, "contact")
  {
    mail.set_subject("Message recu depuis votre site web");
    mail.set_sender(controller.get_page_settings().get_sender_address());
    mail.add_recipient(controller.get_page_settings().get_address());
    mail.set_reply_to(reply_to);
    vars["layout"] = std::string();
  }

  void on_error_occured(const std::exception& error)
  {
    reinterpret_cast<ContactController*>(controller.get())->submit_error();
  }
};
