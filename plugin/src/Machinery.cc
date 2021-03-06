/** @file
   Non-core directive implementations.

 * Copyright 2019, Oath Inc.
 * SPDX-License-Identifier: Apache-2.0
*/

#include "txn_box/common.h"

#include <swoc/TextView.h>
#include <swoc/Errata.h>
#include <swoc/ArenaWriter.h>
#include <swoc/BufferWriter.h>
#include <swoc/bwf_base.h>

#include "txn_box/Directive.h"
#include "txn_box/Comparison.h"
#include "txn_box/Config.h"
#include "txn_box/Context.h"

#include "txn_box/yaml_util.h"
#include "txn_box/ts_util.h"

using swoc::TextView;
using swoc::Errata;
using swoc::Rv;
using swoc::BufferWriter;
namespace bwf = swoc::bwf;
using namespace swoc::literals;

/* ------------------------------------------------------------------------------------ */
Feature Generic::extract() const { return NIL_FEATURE; }
/* ------------------------------------------------------------------------------------ */
class Do_ua_req_url_host : public Directive {
  using super_type = Directive;
  using self_type = Do_ua_req_url_host;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression.
   */
  Do_ua_req_url_host(Expr && expr);

  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
protected:
  Expr _expr; ///< Feature expression.
};

const std::string Do_ua_req_url_host::KEY {"ua-req-url-host" };
const HookMask Do_ua_req_url_host::HOOKS = MaskFor(Hook::PREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP);

Do_ua_req_url_host::Do_ua_req_url_host(Expr && expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_url_host::invoke(Context &ctx) {
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    if (auto url{hdr.url()}; url.is_valid()) {
      auto value = ctx.extract(_expr);
      if (auto host = std::get_if<IndexFor(STRING)>(&value); nullptr != host) {
        url.host_set(*host);
      }
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_url_host::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type{std::move(expr)});
}

// ---

class Do_proxy_req_url_host : public Directive {
  using super_type = Directive;
  using self_type = Do_proxy_req_url_host;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression.
   */
  explicit Do_proxy_req_url_host(Expr && expr);

  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
protected:
  Expr _expr; ///< Host feature expression.
};

const std::string Do_proxy_req_url_host::KEY {"proxy-req-url-host" };
const HookMask Do_proxy_req_url_host::HOOKS {MaskFor({ Hook::PREQ }) };

Do_proxy_req_url_host::Do_proxy_req_url_host(Expr && expr) : _expr(std::move(expr)) {}

Errata Do_proxy_req_url_host::invoke(Context &ctx) {
  if (auto hdr{ctx.proxy_req_hdr()}; hdr.is_valid()) {
    if (auto url{hdr.url()}; url.is_valid()) {
      auto value = ctx.extract(_expr);
      if (auto host = std::get_if<IndexFor(STRING)>(&value); nullptr != host) {
        url.host_set(*host);
      }
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_url_host::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type{std::move(expr)});
}

/* ------------------------------------------------------------------------------------ */
class Do_ua_req_url_port : public Directive {
  using super_type = Directive;
  using self_type = Do_ua_req_url_port;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression.
   */
  Do_ua_req_url_port(Expr && expr);

  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
protected:
  Expr _expr; ///< Feature expression.
};

const std::string Do_ua_req_url_port::KEY {"ua-req-url-port" };
const HookMask Do_ua_req_url_port::HOOKS {MaskFor({Hook::CREQ, Hook::PREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_url_port::Do_ua_req_url_port(Expr && expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_url_port::invoke(Context &ctx) {
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    if (auto url{hdr.url()}; url.is_valid()) {
      auto value = ctx.extract(_expr);
      if (auto port = std::get_if<IndexFor(INTEGER)>(&value); nullptr != port) {
        url.port_set(*port);
      }
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_url_port::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(INTEGER)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), INTEGER);
  }
  return Handle(new self_type{std::move(expr)});
}

// ---

class Do_proxy_req_url_port : public Directive {
  using super_type = Directive;
  using self_type = Do_proxy_req_url_port;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression.
   */
  Do_proxy_req_url_port(Expr && expr);

  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
protected:
  Expr _expr; ///< Feature expression.
};

const std::string Do_proxy_req_url_port::KEY {"proxy-req-url-port" };
const HookMask Do_proxy_req_url_port::HOOKS {MaskFor(Hook::PREQ) };

Do_proxy_req_url_port::Do_proxy_req_url_port(Expr && expr) : _expr(std::move(expr)) {}

Errata Do_proxy_req_url_port::invoke(Context &ctx) {
  if (auto hdr{ctx.proxy_req_hdr()}; hdr.is_valid()) {
    if (auto url{hdr.url()}; url.is_valid()) {
      auto value = ctx.extract(_expr);
      if (auto port = std::get_if<IndexFor(INTEGER)>(&value); nullptr != port) {
        url.port_set(*port);
      }
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_url_port::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(INTEGER)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), INTEGER);
  }
  return Handle(new self_type{std::move(expr)});
}

/* ------------------------------------------------------------------------------------ */
/** Set the host for the request.
 * This updates both the URL and the "Host" field, if appropriate.
 */
class Do_ua_req_host : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_ua_req_host; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression.
   */
  Do_ua_req_host(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Host feature.
};

const std::string Do_ua_req_host::KEY {"ua-req-host" };
const HookMask Do_ua_req_host::HOOKS {MaskFor({Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_host::Do_ua_req_host(Expr &&expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_host::invoke(Context &ctx) {
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    auto value = ctx.extract(_expr);
    if (auto host = std::get_if<IndexFor(STRING)>(&value); nullptr != host) {
      hdr.host_set(*host);
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_host::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the host for the request.
 * This updates both the URL and the "Host" field, if appropriate.
 */
class Do_proxy_req_host : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_proxy_req_host; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_proxy_req_host(Expr && fmt);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _fmt; ///< Host feature.
};

const std::string Do_proxy_req_host::KEY {"proxy-req-host" };
const HookMask Do_proxy_req_host::HOOKS {MaskFor({ Hook::PREQ }) };

Do_proxy_req_host::Do_proxy_req_host(Expr &&fmt) : _fmt(std::move(fmt)) {}

Errata Do_proxy_req_host::invoke(Context &ctx) {
  TextView host{std::get<IndexFor(STRING)>(ctx.extract(_fmt))};
  if (auto hdr{ctx.proxy_req_hdr()}; hdr.is_valid()) {
    hdr.host_set(host);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_host::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a string.)", KEY, drtv_node.Mark());
  }
  return  Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the scheme for the inbound request.
 */
class Do_ua_req_scheme : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_ua_req_scheme; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_ua_req_scheme(Expr && fmt);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Scheme expression.
};

const std::string Do_ua_req_scheme::KEY {"ua-req-scheme" };
const HookMask Do_ua_req_scheme::HOOKS {MaskFor({ Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_scheme::Do_ua_req_scheme(Expr &&fmt) : _expr(std::move(fmt)) {}

Errata Do_ua_req_scheme::invoke(Context &ctx) {
  TextView text{std::get<IndexFor(STRING)>(ctx.extract(_expr))};
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    hdr.url().scheme_set(text);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_scheme::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the URL for the inbound request.
 */
class Do_ua_req_url : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_ua_req_url; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_ua_req_url(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< URL expression.
};

const std::string Do_ua_req_url::KEY {"ua-req-url" };
const HookMask Do_ua_req_url::HOOKS {MaskFor({ Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_url::Do_ua_req_url(Expr &&expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_url::invoke(Context &ctx) {
  TextView text{std::get<IndexFor(STRING)>(ctx.extract(_expr))};
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    hdr.url_set(text);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_url::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the scheme for the outbound request.
 */
class Do_proxy_req_scheme : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_proxy_req_scheme; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_proxy_req_scheme(Expr && fmt);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _fmt; ///< Host feature.
};

const std::string Do_proxy_req_scheme::KEY {"proxy-req-scheme" };
const HookMask Do_proxy_req_scheme::HOOKS {MaskFor({ Hook::PREQ}) };

Do_proxy_req_scheme::Do_proxy_req_scheme(Expr &&fmt) : _fmt(std::move(fmt)) {}

Errata Do_proxy_req_scheme::invoke(Context &ctx) {
  TextView host{std::get<IndexFor(STRING)>(ctx.extract(_fmt))};
  if (auto hdr{ctx.proxy_req_hdr()}; hdr.is_valid()) {
    hdr.url().scheme_set(host);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_scheme::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the URL for the outbound request.
 */
class Do_proxy_req_url : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_proxy_req_url; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression for the URL.
   */
  Do_proxy_req_url(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< URL expression.
};

const std::string Do_proxy_req_url::KEY {"proxy-req-url" };
const HookMask Do_proxy_req_url::HOOKS {MaskFor({ Hook::PREQ }) };

Do_proxy_req_url::Do_proxy_req_url(Expr &&expr) : _expr(std::move(expr)) {}

Errata Do_proxy_req_url::invoke(Context &ctx) {
  TextView text{std::get<IndexFor(STRING)>(ctx.extract(_expr))};
  if (auto hdr{ctx.proxy_req_hdr()}; hdr.is_valid()) {
    hdr.url_set(text);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_url::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a {}.)", KEY, drtv_node.Mark(), STRING);
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
class Do_did_remap : public Directive {
  using self_type = Do_did_remap;
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature expression..
   *
   * @param expr Feature expression for the URL.
   */
  Do_did_remap(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
protected:
  Expr _expr; ///< Boolean to set whether remap was done.
};

const std::string Do_did_remap::KEY {"did-remap" };
const HookMask Do_did_remap::HOOKS {MaskFor(Hook::REMAP) };

Do_did_remap::Do_did_remap(Expr && expr) : _expr(std::move(expr)) {}

Errata Do_did_remap::invoke(Context& ctx) {
  auto f = ctx.extract(_expr);
  ctx._remap_status = f.as_bool() ? TSREMAP_DID_REMAP : TSREMAP_NO_REMAP;
  return {};
}

Rv<Directive::Handle> Do_did_remap::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  // Default, with no value, is @c true.
  if (key_value.IsNull()) {
    return Handle{new self_type(Expr(true))};
  }
  auto && [ expr, errata ] { cfg.parse_expr(key_value)};
  if (!errata.is_ok()) {
    errata.info(R"(While parsing value of "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(BOOLEAN)) {
    return Error(R"(Value for "{}" directive at {} must be convertible to a {}.)", KEY, drtv_node.Mark(), BOOLEAN);
  }
  return Handle{new self_type{std::move(expr)}};
}

/* ------------------------------------------------------------------------------------ */
/** Do the remap.
 */
class Do_apply_remap_rule : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_apply_remap_rule; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
};

const std::string Do_apply_remap_rule::KEY { "apply-remap-rule" };
const HookMask Do_apply_remap_rule::HOOKS { MaskFor(Hook::REMAP) };

Errata Do_apply_remap_rule::invoke(Context &ctx) {
  ctx._remap_status = TSREMAP_DID_REMAP;
  // This is complex because the internal logic is as well. A bit fragile, but this is
  // really only useful as a backwards compatibility fix for pre ATS 9 and should eventually
  // be removed.
  // Copy over the host and port.
  ts::URL replacement_url { ctx._remap_info->requestBufp, ctx._remap_info->mapToUrl };
  ts::URL target_url { ctx._remap_info->requestBufp, ctx._remap_info->mapFromUrl };
  ts::URL request_url { ctx._remap_info->requestBufp, ctx._remap_info->requestUrl };

  in_port_t port = replacement_url.port();
  // decanonicalize the port - may need to dig in and see if it was explicitly set.
  if ((port == 80 && replacement_url.scheme() == ts::URL_SCHEME_HTTP) ||
      (port == 443 && replacement_url.scheme() == ts::URL_SCHEME_HTTPS)) {
    port = 0;
  }
  request_url.port_set(port);
  request_url.host_set(replacement_url.host());
  if (ts::HttpRequest{ctx._remap_info->requestBufp, ctx._remap_info->requestHdrp}.method() != "CONNECT"_tv) {
    request_url.scheme_set(replacement_url.scheme());
    // Update the path as needed.
    auto replacement_path { replacement_url.path() };
    auto target_path { target_url.path() };
    auto request_path { request_url.path() };

    // Need to do better - see if Context can provide an ArenaWriter?
    swoc::LocalBufferWriter<(1<<16) - 1> url_w;
    url_w.write(replacement_path);
    if (request_path.size() > target_path.size()) {
      // Always slash separate the replacement from the remnant of the incoming request path.
      if (url_w.size() && url_w.view()[url_w.size()-1] != '/') {
        url_w.write('/');
      }
      // Already have the separating slash, trim it from the target path.
      url_w.write(request_path.substr(target_path.size()).ltrim('/'));
    }
    request_url.path_set(TextView{url_w.view()}.ltrim('/'));
  };

  return {};
}

swoc::Rv<Directive::Handle> Do_apply_remap_rule::load(Config&, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const&, YAML::Node) {
  return Handle(new self_type);
}
/* ------------------------------------------------------------------------------------ */
/** Set the path for the request.
 */
class Do_ua_req_path : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_ua_req_path; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a expr.
   *
   * @param expr Feature for host.
   */
  Do_ua_req_path(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Host feature.
};

const std::string Do_ua_req_path::KEY {"ua-req-path" };
const HookMask Do_ua_req_path::HOOKS {MaskFor({ Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_path::Do_ua_req_path(Expr &&expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_path::invoke(Context &ctx) {
  auto value { ctx.extract(_expr)};
  if (auto text = std::get_if<IndexFor(STRING)>(&value) ; text ) {
    if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
      hdr.url().path_set(*text);
    }
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_path::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (!expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a string.)", KEY, drtv_node.Mark());
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the query for the request.
 */
class Do_ua_req_query : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_ua_req_query; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param expr Feature for host.
   */
  Do_ua_req_query(Expr && expr);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Host feature.
};

const std::string Do_ua_req_query::KEY {"ua-req-query" };
const HookMask Do_ua_req_query::HOOKS {MaskFor({ Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Do_ua_req_query::Do_ua_req_query(Expr &&expr) : _expr(std::move(expr)) {}

Errata Do_ua_req_query::invoke(Context &ctx) {
  TextView text{std::get<IndexFor(STRING)>(ctx.extract(_expr))};
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    hdr.url().query_set(text);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_ua_req_query::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (expr.is_null()) {
    expr= Feature{FeatureView::Literal(""_tv)};
  }
  if (!expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a string.)", KEY, drtv_node.Mark());
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the path for the request.
 */
class Do_proxy_req_path : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_proxy_req_path; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_proxy_req_path(Expr && fmt);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _fmt; ///< Host feature.
};

const std::string Do_proxy_req_path::KEY {"proxy-req-path" };
const HookMask Do_proxy_req_path::HOOKS {MaskFor({ Hook::PREQ }) };

Do_proxy_req_path::Do_proxy_req_path(Expr &&fmt) : _fmt(std::move(fmt)) {}

Errata Do_proxy_req_path::invoke(Context &ctx) {
  TextView host{std::get<IndexFor(STRING)>(ctx.extract(_fmt))};
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    hdr.url().path_set(host);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_path::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (!expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a string.)", KEY, drtv_node.Mark());
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** Set the query for the request.
 */
class Do_proxy_req_query : public Directive {
  using super_type = Directive; ///< Parent type.
  using self_type = Do_proxy_req_query; ///< Self reference type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  /** Construct with feature extractor @a fmt.
   *
   * @param fmt Feature for host.
   */
  Do_proxy_req_query(Expr && fmt);

  /** Invoke directive.
   *
   * @param ctx Transaction context.
   * @return Errors, if any.
   */
  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _fmt; ///< Host feature.
};

const std::string Do_proxy_req_query::KEY {"proxy-req-query" };
const HookMask Do_proxy_req_query::HOOKS {MaskFor({ Hook::PREQ}) };

Do_proxy_req_query::Do_proxy_req_query(Expr &&fmt) : _fmt(std::move(fmt)) {}

Errata Do_proxy_req_query::invoke(Context &ctx) {
  TextView text{std::get<IndexFor(STRING)>(ctx.extract(_fmt))};
  if (auto hdr{ctx.ua_req_hdr()}; hdr.is_valid()) {
    hdr.url().query_set(text);
  }
  return {};
}

swoc::Rv<Directive::Handle> Do_proxy_req_query::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }
  if (!expr.result_type().can_satisfy(STRING)) {
    return Error(R"(Value for "{}" directive at {} must be a string.)", KEY, drtv_node.Mark());
  }
  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
class FieldDirective : public Directive {
  using self_type = FieldDirective; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
protected:
  TextView _name; ///< Field name.
  Expr _expr; ///< Value for field.

  /** Base constructor.
   *
   * @param name Name of the field.
   * @param expr Value to assign to the field.
   */
  FieldDirective(TextView const& name, Expr && expr);

  /** Load from configuration.
   *
   * @param cfg Configuration.
   * @param maker Subclass maker.
   * @param key Name of the key identifying the directive.
   * @param name Field name (directive argumnet).
   * @param key_value  Value of the node for @a key.
   * @return An instance of the directive for @a key, or errors.
   */
  static Rv<Handle> load(Config& cfg, std::function<Handle (TextView const& name, Expr && fmt)> const& maker, TextView const& key, TextView const& name, YAML::Node key_value);

  using super_type::invoke;
  /** Invoke the directive.
   *
   * @param ctx Runtime context.
   * @param hdr HTTP header containing the field.
   * @return Errors, if any.
   */
  Errata invoke(Context& ctx, ts::HttpHeader && hdr);

  void apply(Context& ctx, ts::HttpHeader & hdr, TextView const& name);

  /** Get the directive name (key).
   *
   * @return The directive key.
   *
   * Used by subclasses to provide the key for diagnostics.
   */
  virtual swoc::TextView key() const = 0;

  template < typename T > void apply(Context&, ts::HttpHeader &&, T const&) {}

  /// Visitor to perform the assignment.
  struct Apply {
    Context & _ctx; ///< Runtime context.
    ts::HttpHeader & _hdr; ///< HTTP Header to modify.
    ts::HttpField _field; ///< Working field.
    TextView const& _name; ///< Field name.

    Apply(Context & ctx, ts::HttpHeader & hdr, TextView const& name) : _ctx(ctx), _hdr(hdr), _field(hdr.field(name)), _name(name) {}

    /// Clear all duplicate fields past @a _field.
    void clear_dups() {
      if (_field.is_valid()) {
        for (auto nf = _field.next_dup(); nf.is_valid();) {
          nf.destroy();
        }
      }
    }

    /// Assigned @a text to a single field, creating as needed.
    /// @return @c true if a field value was changed, @c false if the current value is @a text.
    void assign(TextView const& text) {
      if (_field.is_valid()) {
        if (_field.value() != text) {
          _field.assign(text);
        }
      } else {
        _hdr.field_create(_name).assign(text);
      }
    }

    /// Nil / NULL means destroy the field.
    void operator()(feature_type_for<NIL>) {
      if (_field.is_valid()) {
        this->clear_dups();
        _field.destroy();
      }
    }

    /// Assign the string, clear out any dups.
    void operator()(feature_type_for<STRING>& text) {
      this->assign(text);
      this->clear_dups();
    }

    /// Assign the tuple elements to duplicate fields.
    void operator()(feature_type_for<TUPLE>& t) {
      for (auto const& tf : t) {
        auto text { std::get<STRING>(tf.join(_ctx, ", "_tv)) };
        // skip to next equal field, destroying mismatched fields.
        // once @a _field becomes invalid, it remains in that state.
        while (_field.is_valid() && _field.value() != text) {
          auto tmp = _field.next_dup();
          _field.destroy();
          _field = std::move(tmp);
        }
        this->assign(text);
        _field = _field.next_dup(); // does nothing if @a _field is invalid.
      }
      if (_field.is_valid()) {
        this->clear_dups(); // Any remaining fields need to be cleaned up.
        _field.destroy();
      }
    }

    // Other types, convert to string
    template<typename T> auto operator()(T&& t) -> EnableForFeatureTypes<T, void> {
      swoc::ArenaWriter w{*(_ctx._arena)};
      bwformat(w, bwf::Spec::DEFAULT, t);
      this->assign(w.view());
      this->clear_dups();
    }
  };

};

FieldDirective::FieldDirective(TextView const &name, Expr &&expr) : _name(name), _expr(std::move(expr)) {}

void FieldDirective::apply(Context & ctx, ts::HttpHeader & hdr, TextView const& name) {
  auto value{ctx.extract(_expr)};
  std::visit(Apply{ctx, hdr, name}, value);
}

Errata FieldDirective::invoke(Context & ctx, ts::HttpHeader && hdr) {
  if (hdr.is_valid()) {
    ts::HttpField field;
    this->apply(ctx, hdr, _name);
    return {};
  }
  return Errata().error(R"(Failed to assign field value due to invalid HTTP header.)");
}

auto FieldDirective::load(Config &cfg
                         , std::function<Handle(TextView const &, Expr &&)> const &maker
                         , TextView const &key, TextView const &arg
                         , YAML::Node key_value
                         ) -> Rv<Handle> {
  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing value for "{}".)", key);
    return std::move(errata);
  }

  auto expr_type = expr.result_type();
  if (! expr_type.has_value()) {
    return Error(R"(Directive "{}" must have a value.)", key);
  }
  if (!expr_type.can_satisfy({ NIL, STRING, IP_ADDR, BOOLEAN, FLOAT, INTEGER, ActiveType::TupleOf(STRING)})) {
    return Error(R"(Value for "{}" directive at {} must be a NULL, a string or a list of strings.)", key, key_value.Mark());
  }

  return maker(cfg.localize(arg), std::move(expr));
}

// -- Implementations --

// --
class Do_ua_req_field : public FieldDirective {
  using self_type = Do_ua_req_field;
  using super_type = FieldDirective;
public:
  static const std::string KEY; ///< Directive key.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  using super_type::super_type; // Inherit super_type constructors.
  TextView key() const override { return KEY; }
};

const std::string Do_ua_req_field::KEY {"ua-req-field" };
const HookMask Do_ua_req_field::HOOKS {MaskFor({ Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP }) };

Errata Do_ua_req_field::invoke(Context &ctx) {
  return this->super_type::invoke(ctx, ctx.ua_req_hdr());
}

Rv<Directive::Handle> Do_ua_req_field::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  return super_type::load(cfg, [](TextView const& name, Expr && fmt) -> Handle { return Handle(new self_type(name, std::move(fmt))); }, KEY, arg, key_value);
}

// --
class Do_proxy_req_field : public FieldDirective {
  using self_type = Do_proxy_req_field;
  using super_type = FieldDirective;
public:
  static const std::string KEY; ///< Directive key.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  using super_type::super_type; // Inherit super_type constructors.
  TextView key() const override { return KEY; }
};

const std::string Do_proxy_req_field::KEY {"proxy-req-field" };
const HookMask Do_proxy_req_field::HOOKS {MaskFor({ Hook::PREQ }) };

Errata Do_proxy_req_field::invoke(Context &ctx) {
  return this->super_type::invoke(ctx, ctx.proxy_req_hdr());
}

Rv<Directive::Handle> Do_proxy_req_field::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  return super_type::load(cfg, [](TextView const& name, Expr && fmt) -> Handle { return Handle(new self_type(name, std::move(fmt))); }, KEY, arg, key_value);
}

// --
class Do_proxy_rsp_field : public FieldDirective {
  using self_type = Do_proxy_rsp_field;
  using super_type = FieldDirective;
public:
  static const std::string KEY; ///< Directive key.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  using super_type::super_type; // Inherit super_type constructors.
  TextView key() const override { return KEY; }
};

const std::string Do_proxy_rsp_field::KEY {"proxy-rsp-field" };
const HookMask Do_proxy_rsp_field::HOOKS {MaskFor(Hook::PRSP) };

Errata Do_proxy_rsp_field::invoke(Context &ctx) {
  return this->super_type::invoke(ctx, ctx.proxy_rsp_hdr());
}

Rv<Directive::Handle> Do_proxy_rsp_field::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  return super_type::load(cfg, [](TextView const& name, Expr && fmt) -> Handle { return Handle(new self_type(name, std::move(fmt))); }, KEY, arg, key_value);
}

// --
class Do_upstream_rsp_field : public FieldDirective {
  using self_type = Do_upstream_rsp_field;
  using super_type = FieldDirective;
public:
  static const std::string KEY; ///< Directive key.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  using super_type::super_type; // Inherit super_type constructors.
  TextView key() const override { return KEY; }
};

const std::string Do_upstream_rsp_field::KEY {"upstream-rsp-field" };
const HookMask Do_upstream_rsp_field::HOOKS {MaskFor(Hook::URSP) };

Errata Do_upstream_rsp_field::invoke(Context &ctx) {
  return this->super_type::invoke(ctx, ctx.upstream_rsp_hdr());
}

Rv<Directive::Handle> Do_upstream_rsp_field::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  return super_type::load(cfg, [](TextView const& name, Expr && fmt) -> Handle { return Handle(new self_type(name, std::move(fmt))); }, KEY, arg, key_value);
}
/* ------------------------------------------------------------------------------------ */
/// Set upstream response status code.
class Do_upstream_rsp_status : public Directive {
  using self_type = Do_upstream_rsp_status; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Return status.

  Do_upstream_rsp_status() = default;
};

const std::string Do_upstream_rsp_status::KEY {"upstream-rsp-status" };
const HookMask Do_upstream_rsp_status::HOOKS {MaskFor({Hook::URSP}) };

Errata Do_upstream_rsp_status::invoke(Context &ctx) {
  int status = TS_HTTP_STATUS_NONE;
  Feature value = ctx.extract(_expr);
  auto vtype = ValueTypeOf(value);
  if (INTEGER == vtype) {
    status = std::get<IndexFor(INTEGER)>(value);
  } else if (TUPLE == vtype) {
    auto t = std::get<IndexFor(TUPLE)>(value);
    if (0 < t.count() && t.count() <= 2) {
      if (ValueTypeOf(t[0]) != INTEGER) {
        return Error(R"(Tuple for "{}" must be an integer and a string.)", KEY);
      }
      status = std::get<IndexFor(INTEGER)>(t[0]);
      if (t.count() == 2) {
        if (ValueTypeOf(t[1]) != STRING) {
          return Error(R"(Tuple for "{}" must be an integer and a string.)", KEY);
        }
        ctx._txn.ursp_hdr().reason_set(std::get<IndexFor(STRING)>(t[1]));
      }
    } else {
      return Error(R"(Tuple for "{}" has {} elements, instead of there required 1 or 2.)", KEY, t.size());
    }
  }
  if (100 <= status && status <= 599) {
    ctx._txn.ursp_hdr().status_set(static_cast<TSHttpStatus>(status));
  } else {
    return Errata().error(R"(Status value {} out of range 100..599 for "{}".)", status
                          , KEY);
  }
  return {};
}

Rv<Directive::Handle> Do_upstream_rsp_status::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  auto self = new self_type;
  Handle handle(self);

  auto expr_type = expr.result_type();
  if (!expr_type.can_satisfy({INTEGER, TUPLE})) {
    return Error(R"(Value for "{}" at {} is not an integer or tuple as required.)", KEY, drtv_node.Mark());
  }
  self->_expr = std::move(expr);
  return handle;
}
/* ------------------------------------------------------------------------------------ */
/// Set upstream response reason phrase.
class Do_upstream_reason : public Directive {
  using self_type = Do_upstream_reason; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML configuration.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  TSHttpStatus _status = TS_HTTP_STATUS_NONE; ///< Return status is literal, 0 => extract at runtime.
  Expr _fmt; ///< Reason phrase.

  Do_upstream_reason() = default;
};

const std::string Do_upstream_reason::KEY {"upstream-reason" };
const HookMask Do_upstream_reason::HOOKS {MaskFor({Hook::URSP}) };

Errata Do_upstream_reason::invoke(Context &ctx) {
  auto value = ctx.extract(_fmt);
  if (STRING != ValueTypeOf(value)) {
    return Error(R"(Value for "{}" is not a string.)", KEY);
  }
  ctx._txn.ursp_hdr().reason_set(std::get<IndexFor(STRING)>(value));
  return {};
}

Rv<Directive::Handle> Do_upstream_reason::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(The value for "{}" must be a string.)", KEY, drtv_node.Mark());
  }
  auto self = new self_type;
  Handle handle(self);

  self->_fmt = std::move(expr);

  return handle;
}
/* ------------------------------------------------------------------------------------ */
/// Set proxy response status code.
class Do_proxy_rsp_status : public Directive {
  using self_type = Do_proxy_rsp_status; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML configuration.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Return status.

  Do_proxy_rsp_status() = default;
};

const std::string Do_proxy_rsp_status::KEY {"proxy-rsp-status" };
const HookMask Do_proxy_rsp_status::HOOKS {MaskFor({Hook::PRSP}) };

Errata Do_proxy_rsp_status::invoke(Context &ctx) {
  int status = TS_HTTP_STATUS_NONE;
  Feature value = ctx.extract(_expr);
  auto vtype = ValueTypeOf(value);
  if (INTEGER == vtype) {
    status = std::get<IndexFor(INTEGER)>(value);
  } else if (TUPLE == vtype) {
    auto t = std::get<IndexFor(TUPLE)>(value);
    if (0 < t.count() && t.count() <= 2) {
      if (ValueTypeOf(t[0]) != INTEGER) {
        return Error(R"(Tuple for "{}" must be an integer and a string.)", KEY);
      }
      status = std::get<IndexFor(INTEGER)>(t[0]);
      if (t.count() == 2) {
        if (ValueTypeOf(t[1]) != STRING) {
          return Error(R"(Tuple for "{}" must be an integer and a string.)", KEY);
        }
        ctx._txn.prsp_hdr().reason_set(std::get<IndexFor(STRING)>(t[1]));
      }
    } else {
      return Error(R"(Tuple for "{}" has {} elements, instead of there required 1 or 2.)", KEY, t.size());
    }
  }
  if (100 <= status && status <= 599) {
    ctx._txn.prsp_hdr().status_set(static_cast<TSHttpStatus>(status));
  } else {
    return Errata().error(R"(Status value {} out of range 100..599 for "{}".)", status, KEY);
  }
  return {};
}

Rv<Directive::Handle> Do_proxy_rsp_status::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  auto self = new self_type;
  Handle handle(self);

  auto expr_type = expr.result_type();
  if (!expr_type.can_satisfy(MaskFor({INTEGER, TUPLE}))) {
    return Error(R"(Value for "{}" at {} is not an integer or tuple as required.)", KEY, drtv_node.Mark());
  }
  self->_expr = std::move(expr);
  return handle;
}
/* ------------------------------------------------------------------------------------ */
/// Set proxy response reason phrase.
class Do_proxy_rsp_reason : public Directive {
  using self_type = Do_proxy_rsp_reason; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML configuration.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  TSHttpStatus _status = TS_HTTP_STATUS_NONE; ///< Return status is literal, 0 => extract at runtime.
  Expr _expr; ///< Reason phrase.

  Do_proxy_rsp_reason() = default;
};

const std::string Do_proxy_rsp_reason::KEY {"proxy-rsp-reason" };
const HookMask Do_proxy_rsp_reason::HOOKS {MaskFor({Hook::PRSP}) };

Errata Do_proxy_rsp_reason::invoke(Context &ctx) {
  auto value = ctx.extract(_expr);
  if (STRING != ValueTypeOf(value)) {
    return Error(R"(Value for "{}" is not a string.)", KEY);
  }
  ctx._txn.prsp_hdr().reason_set(std::get<IndexFor(STRING)>(value));
  return {};
}

Rv<Directive::Handle> Do_proxy_rsp_reason::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(The value for "{}" must be a string.)", KEY, drtv_node.Mark());
  }
  auto self = new self_type;
  Handle handle(self);

  self->_expr = std::move(expr);

  return handle;
}
/* ------------------------------------------------------------------------------------ */
/// Set proxy response (error) body.
class Do_proxy_rsp_body : public Directive {
  using self_type = Do_proxy_rsp_body; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML configuration.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Body content.

  Do_proxy_rsp_body() = default;
};

const std::string Do_proxy_rsp_body::KEY {"proxy-rsp-body" };
const HookMask Do_proxy_rsp_body::HOOKS {MaskFor({Hook::PRSP}) };

Errata Do_proxy_rsp_body::invoke(Context &ctx) {
  TextView body, mime { "text/html" };
  auto value = ctx.extract(_expr);
  if (STRING == ValueTypeOf(value)) {
    body = std::get<IndexFor(STRING)>(value);
  } else if (auto tp = std::get_if<IndexFor(TUPLE)>(&value) ; tp ) {
    if (tp->count() == 2) {
      if (auto ptr = std::get_if<IndexFor(STRING)>(&(*tp)[0]) ; ptr ){
        body = *ptr;
      }
      if (auto ptr = std::get_if<IndexFor(STRING)>(&(*tp)[1]) ; ptr ){
        mime = *ptr;
      }
    } else {
      return Error(R"(Value for "{}" is not a list of length 2.)", KEY);
    }
  } else {
    return Error(R"(Value for "{}" is not a string nor a list.)", KEY);
  }
  ctx._txn.error_body_set(body, mime);
  return {};
}

Rv<Directive::Handle> Do_proxy_rsp_body::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy({ STRING, ActiveType::TupleOf(STRING) })) {
    return Error(R"(The value for "{}" must be a string or a list of two strings.)", KEY, drtv_node.Mark());
  }
  auto self = new self_type;
  Handle handle(self);

  self->_expr = std::move(expr);

  return handle;
}
/* ------------------------------------------------------------------------------------ */
/// Replace the upstream response body with a feature.
class Do_upstream_rsp_body : public Directive {
  using self_type = Do_upstream_rsp_body; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML configuration.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Body content.

  Do_upstream_rsp_body(Expr && expr) : _expr(std::move(expr)) {}
};

const std::string Do_upstream_rsp_body::KEY {"upstream-rsp-body" };
const HookMask Do_upstream_rsp_body::HOOKS {MaskFor({Hook::URSP}) };

Errata Do_upstream_rsp_body::invoke(Context &ctx) {
  auto static transform = [](TSCont contp, TSEvent ev_code, void *) -> int {
    if (TSVConnClosedGet(contp)) {
      TSContDestroy(contp);
      return 0;
    }

    auto in_vio = TSVConnWriteVIOGet(contp);
    switch (ev_code) {
      case TS_EVENT_ERROR:TSContCall(TSVIOContGet(in_vio), TS_EVENT_ERROR, in_vio);
        break;
      case TS_EVENT_VCONN_WRITE_COMPLETE:
        TSVConnShutdown(TSTransformOutputVConnGet(contp), 0, 1);
        break;
      default:
        // Consume all input data.
        auto in_todo = TSVIONTodoGet(in_vio);
        auto in_reader = TSVIOReaderGet(in_vio);
        if (in_reader && in_todo) {
          auto avail = TSIOBufferReaderAvail(in_reader);
          in_todo = std::min(in_todo, avail);
          if (in_todo > 0) {
            TSIOBufferReaderConsume(in_reader, in_todo);
            TSVIONDoneSet(in_vio, TSVIONDoneGet(in_vio) + in_todo);
            TSContCall(TSVIOContGet(in_vio)
                       , (TSVIONTodoGet(in_vio) <= 0) ? TS_EVENT_VCONN_WRITE_COMPLETE
                                                      : TS_EVENT_VCONN_WRITE_READY
                       , in_vio);
          }
          // If the full is there, create the output buffer and write it, then clear it.
          auto view = static_cast<TextView *>(TSContDataGet(contp));
          if (view) {
            auto out_vconn = TSTransformOutputVConnGet(contp);
            auto out_buff = TSIOBufferCreate();
            auto out_vio = TSVConnWrite(out_vconn, contp, TSIOBufferReaderAlloc(out_buff), view->size());
            TSIOBufferWrite(out_buff, view->data(), view->size());
            TSContDataSet(contp, nullptr);
            TSVIOReenable(out_vio);
          }
        }
        break;
    }

    return 0;
  };

  auto value = ctx.extract(_expr);
  auto vtype = ValueTypeOf(value);
  TextView* content = nullptr;
  TextView content_type = "text/html";
  if (STRING == vtype) {
    content = &std::get<IndexFor(STRING)>(value);
  } else if (TUPLE == vtype) {
    auto t = std::get<IndexFor(TUPLE)>(value);
    if (t.size() > 0) {
      if (STRING == ValueTypeOf(t[0])) {
        content = &std::get<IndexFor(STRING)>(t[0]);
        if (t.size() > 1 && STRING == ValueTypeOf(t[1])) {
          content_type = std::get<IndexFor(STRING)>(t[1]);
        }
      }
    }
  }

  if (content) {
    // The view contents are in the transaction data, but the full in the feature is not.
    // Make a copy there and pass its address to the continuation.
    auto ctx_view = ctx.alloc_span<TextView>(1);
    auto cont = TSTransformCreate(transform, ctx._txn);
    ctx_view[0] = *content;
    TSContDataSet(cont, ctx_view.data());
    TSHttpTxnHookAdd(ctx._txn, TS_HTTP_RESPONSE_TRANSFORM_HOOK, cont);
    ctx._txn.ursp_hdr().field_obtain("Content-Type"_tv).assign(content_type);
  }

  return {};
}

Rv<Directive::Handle> Do_upstream_rsp_body::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }
  if (! expr.result_type().can_satisfy(STRING)) {
    return Error(R"(The value for "{}" must be a string.)", KEY, drtv_node.Mark());
  }

  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/// Redirect.
/// Although this could technically be done "by hand", it's common enough to justify
/// a specific directive.
class Do_redirect : public Directive {
  using self_type = Do_redirect; ///< Self reference type.
  using super_type = Directive; ///< Parent type.

  /// Per configuration storage.
  struct CfgInfo {
    ReservedSpan _ctx_span; ///< Reserved span for @c CtxInfo.
  };

  /// Per context information.
  /// This is what is stored in the span @c CfgInfo::_ctx_span
  struct CtxInfo {
    TextView _location; ///< Redirect target location.
  };

public:
  static const std::string KEY; ///< Directive name.
  static const std::string STATUS_KEY; ///< Key for status value.
  static const std::string REASON_KEY; ///< Key for reason value.
  static const std::string LOCATION_KEY; ///< Key for location value.
  inline static const std::string BODY_KEY { "body" }; ///< Key for body.

  static const HookMask HOOKS; ///< Valid hooks for directive.

  /// Specify the required amount of reserved configuration storage.
  static constexpr Options OPTIONS { sizeof(CfgInfo) };

  /// Need to do fixups on a later hook.
  static constexpr Hook FIXUP_HOOK = Hook::PRSP;
  /// Status code to use if not specified.
  static const int DEFAULT_STATUS = TS_HTTP_STATUS_MOVED_PERMANENTLY;

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

  /// Configuration level initialization.
  static Errata cfg_init(Config& cfg, CfgStaticData const * rtti);

protected:
  using index_type = FeatureGroup::index_type;

  FeatureGroup _fg; ///< Support cross references among the keys.

  int _status = 0; ///< Return status is literal, 0 => extract at runtime.
  index_type _status_idx; ///< Return status.
  index_type _reason_idx; ///< Status reason text.
  index_type _location_idx; ///< Location field value.
  index_type _body_idx; ///< Body content of respons.
  /// Bounce from fixup hook directive back to @a this.
  Directive::Handle _set_location{new LambdaDirective([this] (Context& ctx) -> Errata { return this->fixup(ctx); })};

  Errata load_status();

  /// Do post invocation fixup.
  Errata fixup(Context &ctx);
};

const std::string Do_redirect::KEY { "redirect" };
const std::string Do_redirect::STATUS_KEY { "status" };
const std::string Do_redirect::REASON_KEY { "reason" };
const std::string Do_redirect::LOCATION_KEY { "location" };

const HookMask Do_redirect::HOOKS { MaskFor({Hook::PRE_REMAP, Hook::REMAP}) };

Errata Do_redirect::cfg_init(Config& cfg, CfgStaticData const * rtti) {
  auto cfg_info = rtti->_cfg_store.rebind<CfgInfo>().data();
  new (cfg_info) CfgInfo; // Initialize the span.
  // Only one redirect can be effective per transaction, therefore shared state per context is best.
  cfg_info->_ctx_span = cfg.reserve_ctx_storage(sizeof(CtxInfo));
  cfg.reserve_slot(FIXUP_HOOK); // needed to fix up "Location" field in proxy response.
  return {};
}

Errata Do_redirect::invoke(Context& ctx) {
  auto cfg_info = _rtti->_cfg_store.rebind<CfgInfo>().data();
  auto ctx_info = ctx.initialized_storage_for<CtxInfo>(cfg_info->_ctx_span).data();

  // If the Location view is empty, it hasn't been set and therefore the clean up hook
  // hasn't been set either, so need to do that.
  bool need_hook_p = ctx_info->_location.empty();

  // Warm up the FeatureGroup.
  _fg.pre_extract(ctx);

  // Finalize the location and stash it in context storage.
  Feature location = _fg.extract(ctx, _location_idx);
  if ( auto view = std::get_if<IndexFor(STRING)>(&location) ; view ) {
    ctx.commit(location);
    ctx_info->_location = *view;
  } else {
    return Error("{} directive - '{}' was not a string as required.", KEY, LOCATION_KEY);
  }

  // Set the status to prevent the upstream request.
  if (_status) {
    ctx._txn.status_set(static_cast<TSHttpStatus>(_status));
  } else {
    Feature value = _fg.extract(ctx, _status_idx);
    auto && [ status, errata ] { value.as_integer(DEFAULT_STATUS) };
    if (!(100 <= status && status <= 599)) {
      status = DEFAULT_STATUS;
    }
    ctx._txn.status_set(static_cast<TSHttpStatus>(status));
  }
  // Arrange for fixup to get invoked.
  if (need_hook_p) {
    ctx.on_hook_do(FIXUP_HOOK, _set_location.get());
  }
  return {};
}

Errata Do_redirect::fixup(Context &ctx) {
  auto cfg_info = _rtti->_cfg_store.rebind<CfgInfo>().data();
  auto ctx_info = ctx.storage_for(cfg_info->_ctx_span).rebind<CtxInfo>().data();
  auto hdr {ctx.proxy_rsp_hdr() };
  // Set the Location
  auto field { hdr.field_obtain(ts::HTTP_FIELD_LOCATION) };
  field.assign(ctx_info->_location);

  // Set the reason.
  if (_reason_idx != FeatureGroup::INVALID_IDX) {
    auto reason{_fg.extract(ctx, _reason_idx)};
    hdr.reason_set(std::get<IndexFor(STRING)>(reason));
  }
  // Set the body.
  if (_body_idx != FeatureGroup::INVALID_IDX) {
    auto body{_fg.extract(ctx, _body_idx)};
    ctx._txn.error_body_set(std::get<IndexFor(STRING)>(body), "text/html");
  }
  return {};
}

Errata Do_redirect::load_status() {
  _status_idx = _fg.index_of(STATUS_KEY);

  if (_status_idx == FeatureGroup::INVALID_IDX) { // not present, use default value.
    _status = DEFAULT_STATUS;
    return {};
  }

  FeatureGroup::ExprInfo & info = _fg[_status_idx];

  if (info._expr.is_literal()) {
    auto && [ status , errata ] { std::get<Feature>(info._expr._expr).as_integer(DEFAULT_STATUS) };
    _status = status;
    if (! errata.is_ok()) {
      errata.info("While load key '{}' for directive '{}'", STATUS_KEY, KEY);
      return std::move(errata);
    }
    if (!(100 <= status && status <= 599)) {
      return Errata().error(R"(Value for '{}' key in {} directive is not a positive integer 100..599 as required.)", STATUS_KEY, KEY);
    }
  } else {
    auto rtype = info._expr.result_type();
    if (rtype != STRING && rtype != INTEGER) {
      return Errata().error(R"({} is not an integer nor string as required.)", STATUS_KEY);
    }
  }
  return {};
}

Rv<Directive::Handle> Do_redirect::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  Handle handle{new self_type};
  Errata errata;
  auto self = static_cast<self_type *>(handle.get());
  if (key_value.IsScalar()) {
    errata = self->_fg.load_as_scalar(cfg, key_value, LOCATION_KEY);
  } else if (key_value.IsSequence()) {
    errata = self->_fg.load_as_tuple(cfg, key_value, { { STATUS_KEY, FeatureGroup::REQUIRED } , { LOCATION_KEY, FeatureGroup::REQUIRED } });
  } else if (key_value.IsMap()) {
    errata = self->_fg.load(cfg, key_value, { { LOCATION_KEY, FeatureGroup::REQUIRED }, { STATUS_KEY }, { REASON_KEY }, { BODY_KEY } });
  } else {
    return Error(R"(Value for "{}" key at {} is must be a scalar, a list, or a map and is not.)", KEY, key_value.Mark());
  }
  if (! errata.is_ok()) {
    errata.info(R"(While parsing value at {} in "{}" directive at {}.)", key_value.Mark(), KEY, drtv_node.Mark());
    return {{}, std::move(errata)};
  }

  self->_reason_idx = self->_fg.index_of(REASON_KEY);
  self->_body_idx = self->_fg.index_of(BODY_KEY);
  self->_location_idx = self->_fg.index_of(LOCATION_KEY);
  errata.note(self->load_status());

  if (! errata.is_ok()) {
    errata.info(R"(While parsing value at {} in "{}" directive at {}.)", key_value.Mark(), KEY, drtv_node.Mark());
    return { {}, std::move(errata) };
  }

  return { std::move(handle), {} };
}
/* ------------------------------------------------------------------------------------ */
/// Send a debug message.
class Do_debug : public Directive {
  using self_type = Do_debug;
  using super_type = Directive;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _tag;
  Expr _msg;

  Do_debug(Expr && tag, Expr && msg);
};

const std::string Do_debug::KEY {"debug" };
const HookMask Do_debug::HOOKS {MaskFor({Hook::POST_LOAD, Hook::TXN_START, Hook::CREQ, Hook::PREQ, Hook::URSP, Hook::PRSP, Hook::PRE_REMAP, Hook::POST_REMAP, Hook::REMAP }) };

Do_debug::Do_debug(Expr &&tag, Expr &&msg) : _tag(std::move(tag)), _msg(std::move(msg)) {}

Errata Do_debug::invoke(Context &ctx) {
  TextView tag = ctx.extract_view(_tag, { Context::EX_COMMIT, Context::EX_C_STR});
  TextView msg = ctx.extract_view(_msg);
  TSDebug(tag.data(), "%.*s", static_cast<int>(msg.size()), msg.data());
  return {};
}

Rv<Directive::Handle> Do_debug::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  if (key_value.IsScalar()) {
    auto && [ msg_fmt, msg_errata ] = cfg.parse_expr(key_value);
    if (! msg_errata.is_ok()) {
      msg_errata.info(R"(While parsing message at {} for "{}" directive at {}.)", key_value.Mark(), KEY, drtv_node.Mark());
      return { {}, std::move(msg_errata)};
    }
    return { Handle{new self_type{Expr{Config::PLUGIN_TAG}, std::move(msg_fmt)}}, {} };
  } else if (key_value.IsSequence()) {
    if (key_value.size() > 2) {
      return Error(R"(Value for "{}" key at {} is not a list of two strings as required.)", KEY, key_value.Mark());
    } else if (key_value.size() < 1) {
      return Error(R"(The list value for "{}" key at {} does not have at least one string as required.)", KEY, key_value.Mark());
    }
    auto && [ tag_expr, tag_errata ] = cfg.parse_expr(key_value[0]);
    if (!tag_errata.is_ok()) {
      tag_errata.info(R"(While parsing tag at {} for "{}" directive at {}.)", key_value[0].Mark(), KEY, drtv_node.Mark());
      return std::move(tag_errata);
    }
    auto && [ msg_expr, msg_errata ] = cfg.parse_expr(key_value[1]);
    if (!tag_errata.is_ok()) {
      tag_errata.info(R"(While parsing message at {} for "{}" directive at {}.)", key_value[1].Mark(), KEY, drtv_node.Mark());
      return std::move(tag_errata);
    }
    return Handle(new self_type(std::move(tag_expr), std::move(msg_expr)));
  }
  return Error(R"(Value for "{}" key at {} is not a string or a list of strings as required.)", KEY, key_value.Mark());
}

/* ------------------------------------------------------------------------------------ */
class QueryDirective {
public:
  Errata invoke(Context &ctx, Expr& fmt, ts::URL url, TextView key);
};

Errata QueryDirective::invoke(Context &ctx, Expr& fmt, ts::URL url, TextView key) {
  auto feature = ctx.extract(fmt);
  if (key.empty()) {
    url.query_set(std::get<IndexFor(STRING)>(feature));
    return {};
  }

  // Need remnant space therefore this needs to be permanent.
  ctx.commit(feature);

  swoc::ArenaWriter aw{*ctx._arena};
  TextView sep; // last separator found.
  TextView::size_type offset = 0;
  auto query { url.query() };
  // Check each parameter for matching @a _arg.
  while (offset < query.size()) {
    if (query.substr(offset).starts_with(key) &&
        ((query.size() == offset + key.size()) ||
         ("=&;"_tv.find(query[key.size()]) != TextView::npos ))) {
      aw.write(sep).write(query.remove_prefix(offset)); // write out prior query section
      if (!is_nil(feature)) {
        Feature value = car(feature);
        if (is_nil(value)) {
          aw.write(key);
        } else {
          aw.print("{}={}", key, value);
        }
      }
      query.remove_prefix(offset).ltrim_if([](char c){return c != '&' && c != ';';});
      sep = query.take_prefix(1);
      offset = 0;
      feature = cdr(feature);
    } else {
      offset = query.find_first_of(";&"_sv, offset+1);
    }
  }
  if (query) {
    aw.write(sep).write(query);
  }
  while (! is_nil(feature)) {
    Feature value = car(feature);
    if (is_nil(value)) {
      aw.write(key);
    } else {
      aw.print("{}={}", key, value);
    }
    feature = cdr(feature);
  }
  return {};
}

class Do_set_creq_query : public Directive, QueryDirective {
  using self_type = Do_set_creq_query;
  using super_type = Directive;
public:
  static const std::string KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);
  static Rv<Handle> load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const& name, swoc::TextView arg, YAML::Node key_value);

protected:
  TextView _arg;
  Expr _expr;

  Do_set_creq_query(TextView arg, Expr && fmt) : _arg(arg), _expr(std::move(fmt)) {}
};

const std::string Do_set_creq_query::KEY { "set-creq-query" };
const HookMask Do_set_creq_query::HOOKS { MaskFor({Hook::CREQ, Hook::PRE_REMAP}) };

Errata Do_set_creq_query::invoke(Context &ctx) {
  return this->QueryDirective::invoke(ctx, _expr, ctx.ua_req_hdr().url(), _arg);
}

Rv<Directive::Handle> Do_set_creq_query::load(Config &cfg, CfgStaticData const*, YAML::Node drtv_node
                                             , swoc::TextView const &, swoc::TextView arg
                                             , YAML::Node key_value) {

  auto && [ expr, errata ] { cfg.parse_expr(key_value) };
  if (! errata.is_ok()) {
    errata.info(R"(While parsing "{}" directive at {}.)", KEY, drtv_node.Mark());
    return std::move(errata);
  }

  return Handle(new self_type(cfg.localize(arg), std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/// Set the cache key.
class Do_cache_key : public Directive {
  using self_type = Do_cache_key; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _fmt; ///< Cache key.

  Do_cache_key(Expr && fmt) : _fmt(std::move(fmt)) {}
};

const std::string Do_cache_key::KEY { "cache-key" };
const HookMask Do_cache_key::HOOKS { MaskFor({Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP}) };

Errata Do_cache_key::invoke(Context &ctx) {
  auto value = ctx.extract(_fmt);
  ctx._txn.cache_key_assign(std::get<IndexFor(STRING)>(value));
  return {};
}

Rv<Directive::Handle> Do_cache_key::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[fmt, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }

  return Handle(new self_type(std::move(fmt)));
}

/* ------------------------------------------------------------------------------------ */
/// Set a transaction configuration variable override.
class Do_txn_conf : public Directive {
  using self_type = Do_txn_conf; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Value for override.
  ts::TxnConfigVar *_var = nullptr;

  Do_txn_conf(Expr && fmt, ts::TxnConfigVar * var) : _expr(std::move(fmt)), _var(var) {}
};

const std::string Do_txn_conf::KEY { "txn-conf" };
const HookMask Do_txn_conf::HOOKS { MaskFor({Hook::TXN_START, Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP, Hook::PREQ}) };

Errata Do_txn_conf::invoke(Context &ctx) {
  auto value = ctx.extract(_expr);
  if (value.index() == IndexFor(INTEGER)) {
    ctx._txn.override_assign(*_var, std::get<IndexFor(INTEGER)>(value));
  } else if (value.index() == IndexFor(BOOLEAN)) {
    ctx._txn.override_assign(*_var, std::get<IndexFor(BOOLEAN)>(value) ? 1L : 0L);
  } else if (value.index() == IndexFor(STRING)) {
    // Unfortunately although the interface doesn't appear to require C strings, in practice some of
    // the string overridables do (such as client cert file path).
    auto str = ctx.localize_as_c_str(std::get<IndexFor(STRING)>(value));
    ctx._txn.override_assign(*_var, str);
  } else if (value.index() == IndexFor(FLOAT)) {
    ctx._txn.override_assign(*_var, std::get<IndexFor(FLOAT)>(value));
  }
  return {};
}

Rv<Directive::Handle> Do_txn_conf::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  auto txn_var = ts::HttpTxn::find_override(arg);
  if (! txn_var) {
    return Error(R"("{}" is not recognized as an overridable transaction configuration variable.)", arg);
  }
  if (txn_var->type() != TS_RECORDDATATYPE_INT &&
    txn_var->type() != TS_RECORDDATATYPE_STRING &&
    txn_var->type() != TS_RECORDDATATYPE_FLOAT
  ) {
    return Error(R"("{}" is of type "{}" which is not currently supported.)", arg, ts::TSRecordDataTypeNames[txn_var->type()]);
  }
  auto &&[fmt, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }

  return Handle(new self_type(std::move(fmt), txn_var));
}

/* ------------------------------------------------------------------------------------ */
/// Set the address for the upstream.
class Do_upstream_addr : public Directive {
  using self_type = Do_upstream_addr; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Address.
  ts::TxnConfigVar *_var = nullptr;

  Do_upstream_addr(Expr && expr) : _expr(std::move(expr)) {}
};

const std::string Do_upstream_addr::KEY { "upstream-addr" };
const HookMask Do_upstream_addr::HOOKS { MaskFor({Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP, Hook::PREQ}) };

Errata Do_upstream_addr::invoke(Context &ctx) {
  auto value = ctx.extract(_expr);
  if (value.index() == IndexFor(IP_ADDR)) {
    ctx._txn.set_upstream_addr(std::get<IndexFor(IP_ADDR)>(value));
  }
  return {};
}

Rv<Directive::Handle> Do_upstream_addr::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }

  if (! expr.result_type().can_satisfy(IP_ADDR)) {
    return Error(R"(Value for "{}" must be an IP address.)");
  }

  return Handle(new self_type(std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/// Set a transaction local variable.
class Do_var : public Directive {
  using self_type = Do_var; ///< Self reference type.
  using super_type = Directive; ///< Parent type.
public:
  static const std::string KEY; ///< Directive name.
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context & ctx) override; ///< Runtime activation.

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  TextView _name; ///< Variable name.
  Expr _value; ///< Value for variable.

  Do_var(TextView const& arg, Expr && value) : _name(arg), _value(std::move(value)) {}
};

const std::string Do_var::KEY { "var" };
const HookMask Do_var::HOOKS { MaskFor({Hook::CREQ, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP, Hook::PREQ, Hook::URSP, Hook::PRSP}) };

Errata Do_var::invoke(Context &ctx) {
  ctx.store_txn_var(_name, ctx.extract(_value));
  return {};
}

Rv<Directive::Handle> Do_var::load(Config& cfg, CfgStaticData const*, YAML::Node, swoc::TextView const&, swoc::TextView const& arg, YAML::Node key_value) {
  auto &&[expr, errata]{cfg.parse_expr(key_value)};
  if (! errata.is_ok()) {
    return std::move(errata);
  }

  return Handle(new self_type(cfg.localize(arg), std::move(expr)));
}
/* ------------------------------------------------------------------------------------ */
/** @c with directive.
 *
 * This is a core directive that has lots of special properties.
 */
class Do_with : public Directive {
  using super_type = Directive;
  using self_type = Do_with;
public:
  static const std::string KEY;
  static const std::string SELECT_KEY;
  static const std::string FOR_EACH_KEY;
  static const std::string CONTINUE_KEY;
  static const HookMask HOOKS; ///< Valid hooks for directive.

  Errata invoke(Context &ctx) override;

  /** Load from YAML node.
   *
   * @param cfg Configuration data.
   * @param rtti Configuration level static data for this directive.
   * @param drtv_node Node containing the directive.
   * @param name Name from key node tag.
   * @param arg Arg from key node tag.
   * @param key_value Value for directive @a KEY
   * @return A directive, or errors on failure.
   */
  static Rv<Handle> load( Config& cfg, CfgStaticData const* rtti, YAML::Node drtv_node, swoc::TextView const& name
                          , swoc::TextView const& arg, YAML::Node key_value);

protected:
  Expr _expr; ///< Feature expression
  Directive::Handle _do; ///< Explicit actions.

  union {
    uint32_t all = 0;
    struct {
      unsigned for_each_p : 1; ///< Direct action is per tuple element.
      unsigned continue_p : 1; ///< Continue with directives after this - want default to be 0.
    } f;
  } _opt;

  /// A single case in the select.
  struct Case {
    Comparison::Handle _cmp; ///< Comparison to perform.
    Directive::Handle _do; ///< Directives to execute.
  };
  using CaseGroup = std::vector<Case>;
  CaseGroup _cases; ///< List of cases for the select.

  Do_with() = default;

  Errata load_case(Config & cfg, YAML::Node node);
};

const std::string Do_with::KEY {"with" };
const std::string Do_with::SELECT_KEY {"select" };
const std::string Do_with::FOR_EACH_KEY {"for-each" };
const std::string Do_with::CONTINUE_KEY {"continue" };

const HookMask Do_with::HOOKS  {MaskFor({Hook::POST_LOAD, Hook::TXN_START, Hook::CREQ, Hook::PREQ, Hook::URSP, Hook::PRSP, Hook::PRE_REMAP, Hook::POST_REMAP, Hook::REMAP }) };

Errata Do_with::invoke(Context &ctx) {
  Feature feature { ctx.extract(_expr) };
  ctx.commit(feature);
  Feature save { ctx._active };
  ctx._active = feature;

  if (_do) {
    if (_opt.f.for_each_p) {
      ctx._active_ext = feature;
      while (! is_nil(feature)) {
        ctx._active = car(feature);
        ctx.mark_terminal(false);
        _do->invoke(ctx);
        cdr(feature);
      }
      clear(feature);
      ctx._active_ext = NIL_FEATURE;
      // Iteration can potentially modify the extracted feature value, so if there are comparisons
      // reset the feature.
      if (! _cases.empty()) {
        ctx._active = feature = ctx.extract(_expr);
      }
    } else {
      ctx.mark_terminal(false);
      _do->invoke(ctx);
    }
  }

  ctx.mark_terminal(false); // default is continue on.
  for ( auto const& c : _cases ) {
    if (! c._cmp || (*c._cmp)(ctx, feature)) {
      if (c._do) {
        c._do->invoke(ctx);
      }
      ctx.mark_terminal(!_opt.f.continue_p); // successful compare, mark terminal.
      break;
    }
  }
  // Need to restore to previous state if nothing matched.
  clear(ctx._active);
  ctx._active = save;
  return {};
}

swoc::Rv<Directive::Handle> Do_with::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  // Need to parse this first, so the feature type can be determined.
  auto && [ expr, errata ] = cfg.parse_expr(key_value);

  if (!errata.is_ok()) {
    return std::move(errata);
  }

  self_type * self = new self_type;
  Handle handle(self); // for return, and cleanup in case of error.
  self->_expr = std::move(expr);
  auto f_scope = cfg.feature_scope(self->_expr.result_type());

  YAML::Node select_node { drtv_node[SELECT_KEY] };
  if (select_node) {
    if (select_node.IsMap()) {
      errata = self->load_case(cfg, select_node);
      if (!errata.is_ok()) {
        return std::move(errata);
      }
    } else if (select_node.IsSequence()) {
      for (YAML::Node child : select_node) {
        errata = (self->load_case(cfg, child));
        if (!errata.is_ok()) {
          errata.info(R"(While loading "{}" directive at {} in "{}" at {}.)", KEY, drtv_node.Mark()
                       , SELECT_KEY, select_node.Mark());
          return std::move(errata);
        }
      }
    } else {
      return Error(R"(The value for "{}" at {} in "{}" directive at {} is not a list or object.")", SELECT_KEY, select_node.Mark(), KEY, drtv_node.Mark());
    }
  }

  YAML::Node continue_node { drtv_node[CONTINUE_KEY]};
  if (continue_node) {
    self->_opt.f.continue_p = true;
  }

  YAML::Node do_node { drtv_node[DO_KEY] };
  YAML::Node for_each_node { drtv_node[FOR_EACH_KEY]};
  if (do_node && for_each_node) {
    return Error(R"("{}" directive cannot have both "{}" and "{}" as keys - {}.)", DO_KEY, FOR_EACH_KEY, drtv_node.Mark());
  } else if (do_node) {
    auto &&[do_handle, errata]{cfg.parse_directive(do_node)};
    if (errata.is_ok()) {
      self->_do = std::move(do_handle);
    } else {
      errata.info(R"(While parsing "{}" key at {} in selection case at {}.)", DO_KEY, do_node.Mark(), drtv_node.Mark());
      return std::move(errata);
    }
  } else if (for_each_node) {
    auto &&[fe_handle, errata]{cfg.parse_directive(for_each_node)};
    if (errata.is_ok()) {
      self->_do = std::move(fe_handle);
      self->_opt.f.for_each_p = true;
    } else {
      errata.info(R"(While parsing "{}" key at {} in selection case at {}.)", FOR_EACH_KEY, for_each_node.Mark(), drtv_node.Mark());
      return std::move(errata);
    }
  }
  return handle;
}

Errata Do_with::load_case(Config & cfg, YAML::Node node) {
  if (node.IsMap()) {
    Case c;
    YAML::Node do_node { node[DO_KEY]};
    // It's allowed to have no comparison, which is either an empty map or only a DO key.
    // In that case the comparison always matches.
    if (node.size() > 1 || (node.size() == 1 && !do_node)) {
      auto f_scope = cfg.feature_scope(_expr.result_type());
      auto &&[cmp_handle, cmp_errata]{Comparison::load(cfg,  node)};
      if (cmp_errata.is_ok()) {
        c._cmp = std::move(cmp_handle);
      } else {
        return std::move(cmp_errata);
      }
    }

    if (do_node) {
      auto c_scope = cfg.capture_scope((c._cmp ? c._cmp->rxp_group_count() : 0), node.Mark().line);
      auto &&[handle, errata]{cfg.parse_directive(do_node)};
      if (errata.is_ok()) {
        c._do = std::move(handle);
      } else {
        errata.info(R"(While parsing "{}" key at {} in selection case at {}.)", DO_KEY
                     , do_node.Mark(), node.Mark());
        return errata;
      }
    } else {
      c._do.reset(new NilDirective);
    }
    // Everything is fine, update the case load and return.
    _cases.emplace_back(std::move(c));
    return {};
  }
  return Error(R"(The value at {} for "{}" is not an object as required.")", node.Mark(), SELECT_KEY);
}

/* ------------------------------------------------------------------------------------ */
const std::string When::KEY { "when" };
const HookMask When::HOOKS  { MaskFor({Hook::CREQ, Hook::PREQ, Hook::URSP, Hook::PRSP, Hook::PRE_REMAP, Hook::REMAP, Hook::POST_REMAP }) };

When::When(Hook hook_idx, Directive::Handle &&directive) : _hook(hook_idx), _directive(std::move
(directive)) {}

// Put the internal directive in the directive array for the specified hook.
Errata When::invoke(Context &ctx) {
  return ctx.on_hook_do(_hook, _directive.get());
}

swoc::Rv<Directive::Handle> When::load(Config& cfg, CfgStaticData const*, YAML::Node drtv_node, swoc::TextView const&, swoc::TextView const&, YAML::Node key_value) {
  Errata zret;
  if (Hook hook{HookName[key_value.Scalar()]} ; hook != Hook::INVALID) {
    if (YAML::Node do_node{drtv_node[DO_KEY]}; do_node) {
      auto save = cfg._hook;
      cfg._hook = hook;
      auto &&[do_handle, do_errata]{cfg.parse_directive(do_node)};
      cfg._hook = save;
      if (do_errata.is_ok()) {
        cfg.reserve_slot(hook);
        return { Handle{new self_type{hook, std::move(do_handle)}} , {}};
      } else {
        zret.note(do_errata);
        zret.error(R"(Failed to load directive in "{}" at {} in "{}" directive at {}.)", DO_KEY
                   , do_node.Mark(), KEY, key_value.Mark());
      }
    } else {
      zret.error(R"(The required "{}" key was not found in the "{}" directive at {}.")", DO_KEY, KEY
                 , drtv_node.Mark());
    }
  } else {
    zret.error(R"(Invalid hook name "{}" in "{}" directive at {}.)", key_value.Scalar(), When::KEY
               , key_value.Mark());
  }
  return {{}, std::move(zret)};
}

/* ------------------------------------------------------------------------------------ */

namespace {
[[maybe_unused]] bool INITIALIZED = [] () -> bool {
  Config::define<When>();
  Config::define<Do_with>();

  Config::define<Do_ua_req_field>();
  Config::define<Do_ua_req_url>();
  Config::define<Do_ua_req_url>("ua-url-host"_tv); // alias
  Config::define<Do_ua_req_url_host>();
  Config::define<Do_ua_req_url_port>();
  Config::define<Do_ua_req_scheme>();
  Config::define<Do_ua_req_host>();
  Config::define<Do_ua_req_path>();
  Config::define<Do_ua_req_query>();

  Config::define<Do_proxy_req_field>();
  Config::define<Do_proxy_req_url>();
  Config::define<Do_proxy_req_url_host>();
  Config::define<Do_proxy_req_url_port>();
  Config::define<Do_proxy_req_url_host>("proxy-url-host"); // alias
  Config::define<Do_proxy_req_host>();
  Config::define<Do_proxy_req_scheme>();
  Config::define<Do_proxy_req_path>();
  Config::define<Do_proxy_req_query>();

  Config::define<Do_apply_remap_rule>();

  Config::define<Do_upstream_rsp_field>();
  Config::define<Do_upstream_rsp_status>();
  Config::define<Do_upstream_reason>();

  Config::define<Do_upstream_addr>();

  Config::define<Do_proxy_rsp_field>();
  Config::define<Do_proxy_rsp_status>();
  Config::define<Do_proxy_rsp_reason>();
  Config::define<Do_proxy_rsp_body>();

  Config::define<Do_upstream_rsp_body>();

  Config::define<Do_cache_key>();
  Config::define<Do_txn_conf>();
  Config::define<Do_redirect>();
  Config::define<Do_debug>();
  Config::define<Do_var>();

  Config::define<Do_did_remap>();

  return true;
} ();
} // namespace
