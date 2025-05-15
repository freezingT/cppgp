#pragma once

#include <exception>
#include <string>
#include <type_traits>
#include <iostream>

namespace util::exceptions
{

    class Error : public std::exception {
    public:
        Error(){}
        virtual ~Error(){}
        Error(std::string message) {
            msg = message;
        }
        std::string getCustomMessage() const {
            return msg;
        }
        void setCustomMessage(const std::string message) {
            msg = message;
        }
        virtual std::string message() const {
            return std::string("Unknown error: ")+msg;
        }
    protected:
        std::string msg;
    };



    class InconsistentInputError : public Error {
    public:
        InconsistentInputError() : Error() {}
        InconsistentInputError(std::string message) : Error(message){}
        virtual std::string message() const override {
            return std::string("Inconsistent input: ")+msg;
        }
    };



    class FileReaderError: public Error {
        public:
        FileReaderError(): Error() {
            filename = "";
        }
        FileReaderError(const std::string& filename): Error() {
            this->filename = filename;
        }
        virtual std::string message() const override {
            if(this->filename.size() > 0){
                return std::string("Unable to open file '")+filename+std::string("'.");
            }
            else {
                return std::string("Unable to open file.");
            }
        }
        private:
        std::string filename;
    };



    template<class T>
    void throwException(const std::string message)
    {
        T exc(message);
        std::cout << exc.message() << std::endl;
        throw exc;
    }


} // namespace util::exceptions
