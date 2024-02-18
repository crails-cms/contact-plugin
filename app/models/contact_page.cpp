#include "contact_page.hpp"
#include "lib/contact_page-odb.hxx"

odb_instantiable_impl(ContactPage)

const std::string ContactPage::scope = "contact_page";
const std::string ContactPage::plural_scope = "contact_pages";
const std::string ContactPage::view = "contact/show";

void ContactPage::edit(Data params)
{
  if (params["sender"].exists())
    sender = params["sender"].as<std::string>();
  if (params["address"].exists())
    address = params["address"].as<std::string>();
  if (params["content"].exists())
    content = params["content"].as<std::string>();
}

void ContactPage::merge_data(Data out)
{
  out["sender"] = sender;
  out["address"] = address;
  out["content"] = content;
}
