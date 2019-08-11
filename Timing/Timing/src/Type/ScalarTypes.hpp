#pragma once

//-----------------------------------------------------------------------------
// External Includes
//-----------------------------------------------------------------------------

// int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t
#include <cstdint>

//-----------------------------------------------------------------------------
// Type Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage
{
	//-------------------------------------------------------------------------
	// Floating Point Scalars
	//-------------------------------------------------------------------------

	/**
	 A floating point type with width of exactly 32 bits.
	 */
	using F32 = float;

	/**
	 A floating point type with width of exactly 64 bits.
	 */
	using F64 = double;

	//-------------------------------------------------------------------------
	// Signed Integer Scalars
	//-------------------------------------------------------------------------

	/**
	 A signed integer type with width of exactly 8 bits.
	 */
	using S8 = std::int8_t;

	/**
	 A signed integer type with width of exactly 16 bits.
	 */
	using S16 = std::int16_t;

	/**
	 A signed integer type with width of exactly 32 bits.
	 */
	using S32 = std::int32_t;

	/**
	 A signed integer type with width of exactly 64 bits.
	 */
	using S64 = std::int64_t;

	//-------------------------------------------------------------------------
	// Unsigned Integer Scalars
	//-------------------------------------------------------------------------

	/**
	 An unsigned integer type with width of exactly 8 bits.
	 */
	using U8 = std::uint8_t;

	/**
	 An unsigned integer type with width of exactly 16 bits.
	 */
	using U16 = std::uint16_t;

	/**
	 An unsigned integer type with width of exactly 32 bits.
	 */
	using U32 = std::uint32_t;

	/**
	 An unsigned integer type with width of exactly 64 bits.
	 */
	using U64 = std::uint64_t;
}