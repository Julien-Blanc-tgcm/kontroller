#include "dnspacket.h"
#include <cstring>

std::optional<std::pair<DNSPacket, std::uint16_t> > DNSPacket::parse_from(char const* data, uint16_t size)
{
	if (size < 12)
		return {};
	DNSPacket reply;
	std::uint16_t tmp;
	std::memcpy(&tmp, data, 2);
	reply.id_ = be16toh(tmp);
	std::memcpy(&tmp, data + 2, 2);
	reply.flags_ = be16toh(tmp);
	std::memcpy(&tmp, data + 4, 2);
	reply.query_count_ = be16toh(tmp);
	std::memcpy(&tmp, data + 6, 2);
	reply.answers_count_ = be16toh(tmp);
	std::memcpy(&tmp, data + 8, 2);
	reply.authority_count_ = be16toh(tmp);
	std::memcpy(&tmp, data + 10, 2);
	reply.additional_count_ = be16toh(tmp);
	std::uint16_t offset = 12;
	return std::make_pair(reply, offset);
}

uint16_t DNSPacket::id() const
{
	return id_;
}

uint16_t DNSPacket::flags() const
{
	return flags_;
}

uint16_t DNSPacket::query_count() const
{
	return query_count_;
}

uint16_t DNSPacket::answers_count() const
{
	return answers_count_;
}

uint16_t DNSPacket::authority_count() const
{
	return authority_count_;
}

uint16_t DNSPacket::additional_count() const
{
	return additional_count_;
}

std::optional<std::pair<DNSQuery, std::uint16_t> > DNSPacket::parse_next_query(char const* data,
                                                                               std::uint16_t size) const
{
	std::string ret;
	int start = 1;
	if (size == 0)
		return {};
	int len = data[0];
	while (len != 0)
	{
		if (start + len >= size - 1) // outside data, error
			return {};
		ret.append(data + start, len);
		start = start + len;
		len = data[start];
		start += 1;
		ret.push_back('.');
	}
	if (start + 4 >= size)
		return {};
	DNSQuery q;
	std::uint16_t tmp;
	std::memcpy(&tmp, data + start, 2);
	q.type = be16toh(tmp);
	std::memcpy(&tmp, data + start + 2, 2);
	q.qu = be16toh(tmp);
	q.name = ret;
	start += 4;
	return std::make_pair(q, static_cast<std::uint16_t>(start));
}

std::pair<DNSAnswer, uint16_t> DNSPacket::parse_next_answer(const char* data, uint16_t size) const
{
	if (size < 12)
		return make_pair(DNSAnswer{}, 0);
	std::uint16_t tmp;
	memcpy(&tmp, data, 2);
	std::uint16_t name = be16toh(tmp);
	memcpy(&tmp, data + 2, 2);
	std::uint16_t type = be16toh(tmp);
	memcpy(&tmp, data + 4, 2);
	std::uint16_t cache_flush = be16toh(tmp);
	std::uint32_t tmp32;
	memcpy(&tmp32, data + 6, 4);
	std::uint32_t ttl = be32toh(tmp32);
	memcpy(&tmp, data + 10, 2);
	std::uint16_t len = be16toh(tmp);
	switch (type)
	{
		case 0x0c: // PTR
		{
			auto ret = parse_next_answer_ptr_(data + 12, size - 12, name, cache_flush, ttl, len);
			if (!static_cast<bool>(ret))
				break; // error
			return std::make_pair(DNSAnswer{*ret}, len + 12);
		}
		case 0x10: // TXT
		{
			auto ret = parse_next_answer_txt_(data + 12, size - 12, name, cache_flush, ttl, len);
			if (!static_cast<bool>(ret))
				break; // error
			return std::make_pair(DNSAnswer{*ret}, len + 12);
		}
		case 0x21: // SRV
		{
			auto ret = parse_next_answer_srv_(data + 12, size - 12, name, cache_flush, ttl, len);
			if (!static_cast<bool>(ret))
				break; // error
			return std::make_pair(DNSAnswer{*ret}, len + 12);
		}
		case 0x01: // A
		{
			auto ret = parse_next_answer_a_(data + 12, size - 12, name, cache_flush, ttl, len);
			if (!static_cast<bool>(ret))
				break; // error
			return std::make_pair(DNSAnswer{*ret}, len + 12);
		}
		case 0x1C: // AAAA
		{
			// TODO: handle
			break;
		}
		default:
			break;
	}
	return make_pair(DNSAnswer{}, len + 12);
}

std::optional<DNSAnswer_TypePtr> DNSPacket::parse_next_answer_ptr_(const char* data,
                                                                   uint16_t size,
                                                                   uint16_t name,
                                                                   uint16_t cache_flush,
                                                                   uint32_t ttl,
                                                                   uint16_t len) const
{
	if (size < len || len == 0)
		return {};
	int dataSize = static_cast<std::uint8_t>(data[0]);
	if (1 + dataSize >= size)
		return {}; // malformed record
	DNSAnswer_TypePtr answer;
	answer.domain_name.append(data + 1, dataSize);
	answer.name = name;
	answer.cache_flush = cache_flush;
	answer.ttl = ttl;
	return answer;
}

std::optional<DNSAnswer_TypeTxt> DNSPacket::parse_next_answer_txt_(const char* data,
                                                                   uint16_t size,
                                                                   uint16_t name,
                                                                   std::uint16_t cache_flush,
                                                                   std::uint32_t ttl,
                                                                   std::uint16_t len) const
{
	if (size < len || len == 0)
		return {};
	int txtSize = static_cast<std::uint8_t>(data[0]);
	int totalSize = len;
	int offset = 1;
	DNSAnswer_TypeTxt answer;
	while (totalSize > 0)
	{
		if (offset + txtSize >= size)
			return {}; // malformed record
		answer.txts.emplace_back(data + offset, txtSize);
		offset += txtSize + 1;
		totalSize -= txtSize + 1;
		if (totalSize > 0)
			txtSize = static_cast<std::uint8_t>(data[offset - 1]);
	}
	answer.name = name;
	answer.cache_flush = cache_flush;
	answer.ttl = ttl;
	return answer;
}

std::optional<DNSAnswer_TypeSrv> DNSPacket::parse_next_answer_srv_(const char* data,
                                                                   uint16_t size,
                                                                   uint16_t name,
                                                                   uint16_t cache_flush,
                                                                   uint32_t ttl,
                                                                   uint16_t len) const
{
	if (size < len || len <= 7) // needs at least 7 bytes
		return {};
	std::uint16_t tmp16;
	DNSAnswer_TypeSrv answer;
	answer.cache_flush = cache_flush;
	answer.ttl = ttl;
	answer.name = name;
	std::memcpy(&tmp16, data, 2);
	answer.priority = be16toh(tmp16);
	std::memcpy(&tmp16, data + 2, 2);
	answer.weight = be16toh(tmp16);
	std::memcpy(&tmp16, data + 4, 2);
	answer.port = be16toh(tmp16);
	int string_len = static_cast<std::uint8_t>(data[6]);
	if (len < 7 + string_len)
		return {}; // Invalid frame
	answer.target = std::string(data + 7, string_len);
	return answer;
}

std::optional<DNSAnswer_TypeA> DNSPacket::parse_next_answer_a_(const char* data,
                                                               uint16_t size,
                                                               uint16_t name,
                                                               uint16_t cache_flush,
                                                               uint32_t ttl,
                                                               uint16_t len) const
{
	if (size < len || len != 4) // needs to be 4 bytes
		return {};
	DNSAnswer_TypeA answer;
	answer.name = name;
	answer.cache_flush = cache_flush;
	answer.ttl = ttl;
	std::uint32_t tmp;
	memcpy(&tmp, data, 4);
	answer.ipAddress = be32toh(tmp);
	return answer;
}
