#pragma once

#include <memory>

namespace gp::util
{

    class Prototype {
    public:
        Prototype();
        virtual ~Prototype();

        virtual std::shared_ptr<Prototype> copy() const = 0;
    };

} // namespace gp::util
