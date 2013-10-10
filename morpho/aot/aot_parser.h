/** Copyright &copy; 2011-2013, Ontologs.
 * \file   aot_parser.h
 * \brief  AOT parser interface definition.
 * \author Vladimir Lapshin.
 * \date   02.04.2013
 */

#pragma once

#include <stdint.h>

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace factor { namespace morpho { namespace aot {

class AotParser : private boost::noncopyable {
public:
  /// Smart pointer type definition.
  typedef boost::shared_ptr<AotParser> Ptr;

  /// Check is passed string main form code.
  virtual uint32_t ParseAttrLine(const std::string& line, std::string& id) const = 0;

  /// Check code correctness.
  virtual bool IsMainFormCode(const std::string& code) const = 0;
};

}}} // namespace factor, morpho, aot.
