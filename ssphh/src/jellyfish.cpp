#include <jellyfish.hpp>


Jellyfish::Jellyfish()
	: Vf::Widget("jellyfish") {

}

Jellyfish::Jellyfish(const std::string& name)
	: Widget(name) {

}

Jellyfish::~Jellyfish() {

}

void Jellyfish::OnInit(const std::vector<std::string>& args) {
	Vf::Widget::OnInit(args);
}

void Jellyfish::OnKill() {

}
