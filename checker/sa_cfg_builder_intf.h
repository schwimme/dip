#pragma once


#include <types/string.h>
#include <memory>


namespace checker
{


struct sa_cfg
{

};


struct sa_cfg_builder_intf
{
	virtual std::shared_ptr<sa_cfg> build(const base::string& path) const = 0;
};


}
