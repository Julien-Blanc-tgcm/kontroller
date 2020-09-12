#ifndef DNSPACKET_H
#define DNSPACKET_H

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

struct DNSQuery
{
	std::uint16_t type;
	std::uint16_t qu;
	std::string name;
};

struct DNSAnswer_TypeA
{
	std::uint16_t name;
	std::uint16_t cache_flush;
	std::uint32_t ttl;
	/**
	 * @brief ipAddress as a 32 bit integer. Follows machine endianness
	 */
	std::uint32_t ipAddress;
};

struct DNSAnswer_TypePtr
{
	std::uint16_t name;
	std::uint16_t cache_flush;
	std::uint32_t ttl;
	std::string domain_name;
};

struct DNSAnswer_TypeTxt
{
	std::uint16_t name;
	std::uint16_t cache_flush;
	std::uint32_t ttl;
	std::vector<std::string> txts;
};

struct DNSAnswer_TypeSrv
{
	std::uint16_t name;
	std::uint16_t cache_flush;
	std::uint32_t ttl;
	std::uint16_t priority;
	std::uint16_t weight;
	std::uint16_t port;
	std::string target;
};

using DNSAnswer =
    std::variant<std::monostate, DNSAnswer_TypeA, DNSAnswer_TypePtr, DNSAnswer_TypeTxt, DNSAnswer_TypeSrv>;

class DNSPacket
{
	DNSPacket() = default;

	std::uint16_t id_;
	std::uint16_t flags_;
	std::uint16_t query_count_;
	std::uint16_t answers_count_;
	std::uint16_t authority_count_;
	std::uint16_t additional_count_;

  public:
	/**
	 * \brief Parses the packet information
	 * \note should use a span
	 * \return empty optional if data could not be parsed. Otherwise, a pair containing the DNS packet and the number
	 * of data consumed
	 */
	static std::optional<std::pair<DNSPacket, std::uint16_t> > parse_from(char const* data, std::uint16_t size);

	std::uint16_t id() const;
	std::uint16_t flags() const;
	std::uint16_t query_count() const;
	std::uint16_t answers_count() const;
	std::uint16_t authority_count() const;
	std::uint16_t additional_count() const;

	/**
	 * \brief parse_next_query returns the next query, an empty optional if a parse error occurred. It updates the
	 * internal offset if successful. No query count is checked (caller's responsibility)
	 * \param data the beginning of the query part (not the beginning of the
	 * \param the remaining size of the whole dns packet
	 * \return
	 */
	std::optional<std::pair<DNSQuery, std::uint16_t> > parse_next_query(char const* data, std::uint16_t size) const;

	std::pair<DNSAnswer, std::uint16_t> parse_next_answer(char const* data, std::uint16_t size) const;

  private:
	std::optional<DNSAnswer_TypePtr> parse_next_answer_ptr_(char const* data,
	                                                        std::uint16_t size,
	                                                        std::uint16_t name,
	                                                        std::uint16_t cache_flush,
	                                                        std::uint32_t ttl,
	                                                        std::uint16_t len) const;

	std::optional<DNSAnswer_TypeTxt> parse_next_answer_txt_(char const* data,
	                                                        std::uint16_t size,
	                                                        std::uint16_t name,
	                                                        std::uint16_t cache_flush,
	                                                        std::uint32_t ttl,
	                                                        std::uint16_t len) const;

	std::optional<DNSAnswer_TypeSrv> parse_next_answer_srv_(char const* data,
	                                                        std::uint16_t size,
	                                                        std::uint16_t name,
	                                                        std::uint16_t cache_flush,
	                                                        std::uint32_t ttl,
	                                                        std::uint16_t len) const;

	std::optional<DNSAnswer_TypeA> parse_next_answer_a_(char const* data,
	                                                    std::uint16_t size,
	                                                    std::uint16_t name,
	                                                    std::uint16_t cache_flush,
	                                                    std::uint32_t ttl,
	                                                    std::uint16_t len) const;
};

#endif // DNSPACKET_H
