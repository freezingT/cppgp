#pragma once

#include <memory>

namespace util
{
    /**
     * Prototype interface class, implementing the Prototype design pattern.
     * It defines a method that allows the creation of a deep copy of an object.
     *
     * @see https://refactoring.guru/design-patterns/prototype
     */
    class Prototype {
    public:
        Prototype();
        virtual ~Prototype();

        /**
         * Create a deep copy of the object.
         *
         * @return The deep copy.
         */
        virtual std::shared_ptr<Prototype> copy() const = 0;
    };

} // namespace util
