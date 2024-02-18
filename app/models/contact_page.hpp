#pragma once
#include <crails/odb/helpers.hpp>
#include <crails/odb/model.hpp>

#pragma db object
class ContactPage : public Crails::Odb::Model
{
  odb_instantiable()
public:
  static const std::string scope;
  static const std::string plural_scope;
  static const std::string view;

  #pragma db view object(ContactPage)
  struct Count
  {
    #pragma db column("count(" + ContactPage::id + ")")
    size_t value;
  };

  void edit(Data);
  void merge_data(Data);

  const std::string& get_sender_address() const { return sender; }
  void set_sender_address(const std::string& value) { sender = value; }
  const std::string& get_content() const { return content; }
  void set_content(const std::string& value) { content = value; }
  const std::string& get_address() const { return address; }
  void set_address(const std::string& value) { address = value; }

private:
  std::string sender;
  std::string address;
  std::string content;
};
