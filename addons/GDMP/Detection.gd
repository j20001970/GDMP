const PROTO_VERSION = 2

#
# BSD 3-Clause License
#
# Copyright (c) 2018 - 2020, Oleg Malyavkin
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# DEBUG_TAB redefine this "  " if you need, example: const DEBUG_TAB = "\t"
const DEBUG_TAB : String = "  "

enum PB_ERR {
	NO_ERRORS = 0,
	VARINT_NOT_FOUND = -1,
	REPEATED_COUNT_NOT_FOUND = -2,
	REPEATED_COUNT_MISMATCH = -3,
	LENGTHDEL_SIZE_NOT_FOUND = -4,
	LENGTHDEL_SIZE_MISMATCH = -5,
	PACKAGE_SIZE_MISMATCH = -6,
	UNDEFINED_STATE = -7,
	PARSE_INCOMPLETE = -8,
	REQUIRED_FIELDS = -9
}

enum PB_DATA_TYPE {
	INT32 = 0,
	SINT32 = 1,
	UINT32 = 2,
	INT64 = 3,
	SINT64 = 4,
	UINT64 = 5,
	BOOL = 6,
	ENUM = 7,
	FIXED32 = 8,
	SFIXED32 = 9,
	FLOAT = 10,
	FIXED64 = 11,
	SFIXED64 = 12,
	DOUBLE = 13,
	STRING = 14,
	BYTES = 15,
	MESSAGE = 16,
	MAP = 17
}

const DEFAULT_VALUES_2 = {
	PB_DATA_TYPE.INT32: null,
	PB_DATA_TYPE.SINT32: null,
	PB_DATA_TYPE.UINT32: null,
	PB_DATA_TYPE.INT64: null,
	PB_DATA_TYPE.SINT64: null,
	PB_DATA_TYPE.UINT64: null,
	PB_DATA_TYPE.BOOL: null,
	PB_DATA_TYPE.ENUM: null,
	PB_DATA_TYPE.FIXED32: null,
	PB_DATA_TYPE.SFIXED32: null,
	PB_DATA_TYPE.FLOAT: null,
	PB_DATA_TYPE.FIXED64: null,
	PB_DATA_TYPE.SFIXED64: null,
	PB_DATA_TYPE.DOUBLE: null,
	PB_DATA_TYPE.STRING: null,
	PB_DATA_TYPE.BYTES: null,
	PB_DATA_TYPE.MESSAGE: null,
	PB_DATA_TYPE.MAP: null
}

const DEFAULT_VALUES_3 = {
	PB_DATA_TYPE.INT32: 0,
	PB_DATA_TYPE.SINT32: 0,
	PB_DATA_TYPE.UINT32: 0,
	PB_DATA_TYPE.INT64: 0,
	PB_DATA_TYPE.SINT64: 0,
	PB_DATA_TYPE.UINT64: 0,
	PB_DATA_TYPE.BOOL: false,
	PB_DATA_TYPE.ENUM: 0,
	PB_DATA_TYPE.FIXED32: 0,
	PB_DATA_TYPE.SFIXED32: 0,
	PB_DATA_TYPE.FLOAT: 0.0,
	PB_DATA_TYPE.FIXED64: 0,
	PB_DATA_TYPE.SFIXED64: 0,
	PB_DATA_TYPE.DOUBLE: 0.0,
	PB_DATA_TYPE.STRING: "",
	PB_DATA_TYPE.BYTES: [],
	PB_DATA_TYPE.MESSAGE: null,
	PB_DATA_TYPE.MAP: []
}

enum PB_TYPE {
	VARINT = 0,
	FIX64 = 1,
	LENGTHDEL = 2,
	STARTGROUP = 3,
	ENDGROUP = 4,
	FIX32 = 5,
	UNDEFINED = 8
}

enum PB_RULE {
	OPTIONAL = 0,
	REQUIRED = 1,
	REPEATED = 2,
	RESERVED = 3
}

enum PB_SERVICE_STATE {
	FILLED = 0,
	UNFILLED = 1
}

class PBField:
	func _init(a_name : String, a_type : int, a_rule : int, a_tag : int, packed : bool, a_value = null):
		name = a_name
		type = a_type
		rule = a_rule
		tag = a_tag
		option_packed = packed
		value = a_value
	var name : String
	var type : int
	var rule : int
	var tag : int
	var option_packed : bool
	var value
	var option_default : bool = false

class PBTypeTag:
	var ok : bool = false
	var type : int
	var tag : int
	var offset : int

class PBServiceField:
	var field : PBField
	var func_ref = null
	var state : int = PB_SERVICE_STATE.UNFILLED

class PBPacker:
	static func convert_signed(n : int) -> int:
		if n < -2147483648:
			return (n << 1) ^ (n >> 63)
		else:
			return (n << 1) ^ (n >> 31)

	static func deconvert_signed(n : int) -> int:
		if n & 0x01:
			return ~(n >> 1)
		else:
			return (n >> 1)

	static func pack_varint(value) -> PoolByteArray:
		var varint : PoolByteArray = PoolByteArray()
		if typeof(value) == TYPE_BOOL:
			if value:
				value = 1
			else:
				value = 0
		for _i in range(9):
			var b = value & 0x7F
			value >>= 7
			if value:
				varint.append(b | 0x80)
			else:
				varint.append(b)
				break
		if varint.size() == 9 && varint[8] == 0xFF:
			varint.append(0x01)
		return varint

	static func pack_bytes(value, count : int, data_type : int) -> PoolByteArray:
		var bytes : PoolByteArray = PoolByteArray()
		if data_type == PB_DATA_TYPE.FLOAT:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			spb.put_float(value)
			bytes = spb.get_data_array()
		elif data_type == PB_DATA_TYPE.DOUBLE:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			spb.put_double(value)
			bytes = spb.get_data_array()
		else:
			for _i in range(count):
				bytes.append(value & 0xFF)
				value >>= 8
		return bytes

	static func unpack_bytes(bytes : PoolByteArray, index : int, count : int, data_type : int):
		var value = 0
		if data_type == PB_DATA_TYPE.FLOAT:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			for i in range(index, count + index):
				spb.put_u8(bytes[i])
			spb.seek(0)
			value = spb.get_float()
		elif data_type == PB_DATA_TYPE.DOUBLE:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			for i in range(index, count + index):
				spb.put_u8(bytes[i])
			spb.seek(0)
			value = spb.get_double()
		else:
			for i in range(index + count - 1, index - 1, -1):
				value |= (bytes[i] & 0xFF)
				if i != index:
					value <<= 8
		return value

	static func unpack_varint(varint_bytes) -> int:
		var value : int = 0
		for i in range(varint_bytes.size() - 1, -1, -1):
			value |= varint_bytes[i] & 0x7F
			if i != 0:
				value <<= 7
		return value

	static func pack_type_tag(type : int, tag : int) -> PoolByteArray:
		return pack_varint((tag << 3) | type)

	static func isolate_varint(bytes : PoolByteArray, index : int) -> PoolByteArray:
		var result : PoolByteArray = PoolByteArray()
		for i in range(index, bytes.size()):
			result.append(bytes[i])
			if !(bytes[i] & 0x80):
				break
		return result

	static func unpack_type_tag(bytes : PoolByteArray, index : int) -> PBTypeTag:
		var varint_bytes : PoolByteArray = isolate_varint(bytes, index)
		var result : PBTypeTag = PBTypeTag.new()
		if varint_bytes.size() != 0:
			result.ok = true
			result.offset = varint_bytes.size()
			var unpacked : int = unpack_varint(varint_bytes)
			result.type = unpacked & 0x07
			result.tag = unpacked >> 3
		return result

	static func pack_length_delimeted(type : int, tag : int, bytes : PoolByteArray) -> PoolByteArray:
		var result : PoolByteArray = pack_type_tag(type, tag)
		result.append_array(pack_varint(bytes.size()))
		result.append_array(bytes)
		return result

	static func pb_type_from_data_type(data_type : int) -> int:
		if data_type == PB_DATA_TYPE.INT32 || data_type == PB_DATA_TYPE.SINT32 || data_type == PB_DATA_TYPE.UINT32 || data_type == PB_DATA_TYPE.INT64 || data_type == PB_DATA_TYPE.SINT64 || data_type == PB_DATA_TYPE.UINT64 || data_type == PB_DATA_TYPE.BOOL || data_type == PB_DATA_TYPE.ENUM:
			return PB_TYPE.VARINT
		elif data_type == PB_DATA_TYPE.FIXED32 || data_type == PB_DATA_TYPE.SFIXED32 || data_type == PB_DATA_TYPE.FLOAT:
			return PB_TYPE.FIX32
		elif data_type == PB_DATA_TYPE.FIXED64 || data_type == PB_DATA_TYPE.SFIXED64 || data_type == PB_DATA_TYPE.DOUBLE:
			return PB_TYPE.FIX64
		elif data_type == PB_DATA_TYPE.STRING || data_type == PB_DATA_TYPE.BYTES || data_type == PB_DATA_TYPE.MESSAGE || data_type == PB_DATA_TYPE.MAP:
			return PB_TYPE.LENGTHDEL
		else:
			return PB_TYPE.UNDEFINED

	static func pack_field(field : PBField) -> PoolByteArray:
		var type : int = pb_type_from_data_type(field.type)
		var type_copy : int = type
		if field.rule == PB_RULE.REPEATED && field.option_packed:
			type = PB_TYPE.LENGTHDEL
		var head : PoolByteArray = pack_type_tag(type, field.tag)
		var data : PoolByteArray = PoolByteArray()
		if type == PB_TYPE.VARINT:
			var value
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						value = convert_signed(v)
					else:
						value = v
					data.append_array(pack_varint(value))
				return data
			else:
				if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
					value = convert_signed(field.value)
				else:
					value = field.value
				data = pack_varint(value)
		elif type == PB_TYPE.FIX32:
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					data.append_array(pack_bytes(v, 4, field.type))
				return data
			else:
				data.append_array(pack_bytes(field.value, 4, field.type))
		elif type == PB_TYPE.FIX64:
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					data.append_array(pack_bytes(v, 8, field.type))
				return data
			else:
				data.append_array(pack_bytes(field.value, 8, field.type))
		elif type == PB_TYPE.LENGTHDEL:
			if field.rule == PB_RULE.REPEATED:
				if type_copy == PB_TYPE.VARINT:
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						var signed_value : int
						for v in field.value:
							signed_value = convert_signed(v)
							data.append_array(pack_varint(signed_value))
					else:
						for v in field.value:
							data.append_array(pack_varint(v))
					return pack_length_delimeted(type, field.tag, data)
				elif type_copy == PB_TYPE.FIX32:
					for v in field.value:
						data.append_array(pack_bytes(v, 4, field.type))
					return pack_length_delimeted(type, field.tag, data)
				elif type_copy == PB_TYPE.FIX64:
					for v in field.value:
						data.append_array(pack_bytes(v, 8, field.type))
					return pack_length_delimeted(type, field.tag, data)
				elif field.type == PB_DATA_TYPE.STRING:
					for v in field.value:
						var obj = v.to_utf8()
						data.append_array(pack_length_delimeted(type, field.tag, obj))
					return data
				elif field.type == PB_DATA_TYPE.BYTES:
					for v in field.value:
						data.append_array(pack_length_delimeted(type, field.tag, v))
					return data
				elif typeof(field.value[0]) == TYPE_OBJECT:
					for v in field.value:
						var obj : PoolByteArray = v.to_bytes()
						#if obj != null && obj.size() > 0:
						#	data.append_array(pack_length_delimeted(type, field.tag, obj))
						#else:
						#	data = PoolByteArray()
						#	return data
						data.append_array(pack_length_delimeted(type, field.tag, obj))
					return data
			else:
				if field.type == PB_DATA_TYPE.STRING:
					var str_bytes : PoolByteArray = field.value.to_utf8()
					if PROTO_VERSION == 2 || (PROTO_VERSION == 3 && str_bytes.size() > 0):
						data.append_array(str_bytes)
						return pack_length_delimeted(type, field.tag, data)
				if field.type == PB_DATA_TYPE.BYTES:
					if PROTO_VERSION == 2 || (PROTO_VERSION == 3 && field.value.size() > 0):
						data.append_array(field.value)
						return pack_length_delimeted(type, field.tag, data)
				elif typeof(field.value) == TYPE_OBJECT:
					var obj : PoolByteArray = field.value.to_bytes()
					#if obj != null && obj.size() > 0:
					#	data.append_array(obj)
					#	return pack_length_delimeted(type, field.tag, data)
					if obj.size() > 0:
						data.append_array(obj)
					return pack_length_delimeted(type, field.tag, data)
				else:
					pass
		if data.size() > 0:
			head.append_array(data)
			return head
		else:
			return data

	static func unpack_field(bytes : PoolByteArray, offset : int, field : PBField, type : int, message_func_ref) -> int:
		if field.rule == PB_RULE.REPEATED && type != PB_TYPE.LENGTHDEL && field.option_packed:
			var count = isolate_varint(bytes, offset)
			if count.size() > 0:
				offset += count.size()
				count = unpack_varint(count)
				if type == PB_TYPE.VARINT:
					var val
					var counter = offset + count
					while offset < counter:
						val = isolate_varint(bytes, offset)
						if val.size() > 0:
							offset += val.size()
							val = unpack_varint(val)
							if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
								val = deconvert_signed(val)
							elif field.type == PB_DATA_TYPE.BOOL:
								if val:
									val = true
								else:
									val = false
							field.value.append(val)
						else:
							return PB_ERR.REPEATED_COUNT_MISMATCH
					return offset
				elif type == PB_TYPE.FIX32 || type == PB_TYPE.FIX64:
					var type_size
					if type == PB_TYPE.FIX32:
						type_size = 4
					else:
						type_size = 8
					var val
					var counter = offset + count
					while offset < counter:
						if (offset + type_size) > bytes.size():
							return PB_ERR.REPEATED_COUNT_MISMATCH
						val = unpack_bytes(bytes, offset, type_size, field.type)
						offset += type_size
						field.value.append(val)
					return offset
			else:
				return PB_ERR.REPEATED_COUNT_NOT_FOUND
		else:
			if type == PB_TYPE.VARINT:
				var val = isolate_varint(bytes, offset)
				if val.size() > 0:
					offset += val.size()
					val = unpack_varint(val)
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						val = deconvert_signed(val)
					elif field.type == PB_DATA_TYPE.BOOL:
						if val:
							val = true
						else:
							val = false
					if field.rule == PB_RULE.REPEATED:
						field.value.append(val)
					else:
						field.value = val
				else:
					return PB_ERR.VARINT_NOT_FOUND
				return offset
			elif type == PB_TYPE.FIX32 || type == PB_TYPE.FIX64:
				var type_size
				if type == PB_TYPE.FIX32:
					type_size = 4
				else:
					type_size = 8
				var val
				if (offset + type_size) > bytes.size():
					return PB_ERR.REPEATED_COUNT_MISMATCH
				val = unpack_bytes(bytes, offset, type_size, field.type)
				offset += type_size
				if field.rule == PB_RULE.REPEATED:
					field.value.append(val)
				else:
					field.value = val
				return offset
			elif type == PB_TYPE.LENGTHDEL:
				var inner_size = isolate_varint(bytes, offset)
				if inner_size.size() > 0:
					offset += inner_size.size()
					inner_size = unpack_varint(inner_size)
					if inner_size >= 0:
						if inner_size + offset > bytes.size():
							return PB_ERR.LENGTHDEL_SIZE_MISMATCH
						if message_func_ref != null:
							var message = message_func_ref.call_func()
							if inner_size > 0:
								var sub_offset = message.from_bytes(bytes, offset, inner_size + offset)
								if sub_offset > 0:
									if sub_offset - offset >= inner_size:
										offset = sub_offset
										return offset
									else:
										return PB_ERR.LENGTHDEL_SIZE_MISMATCH
								return sub_offset
							else:
								return offset
						elif field.type == PB_DATA_TYPE.STRING:
							var str_bytes : PoolByteArray = PoolByteArray()
							for i in range(offset, inner_size + offset):
								str_bytes.append(bytes[i])
							if field.rule == PB_RULE.REPEATED:
								field.value.append(str_bytes.get_string_from_utf8())
							else:
								field.value = str_bytes.get_string_from_utf8()
							return offset + inner_size
						elif field.type == PB_DATA_TYPE.BYTES:
							var val_bytes : PoolByteArray = PoolByteArray()
							for i in range(offset, inner_size + offset):
								val_bytes.append(bytes[i])
							if field.rule == PB_RULE.REPEATED:
								field.value.append(val_bytes)
							else:
								field.value = val_bytes
							return offset + inner_size
					else:
						return PB_ERR.LENGTHDEL_SIZE_NOT_FOUND
				else:
					return PB_ERR.LENGTHDEL_SIZE_NOT_FOUND
		return PB_ERR.UNDEFINED_STATE

	static func unpack_message(data, bytes : PoolByteArray, offset : int, limit : int) -> int:
		while true:
			var tt : PBTypeTag = unpack_type_tag(bytes, offset)
			if tt.ok:
				offset += tt.offset
				if data.has(tt.tag):
					var service : PBServiceField = data[tt.tag]
					var type : int = pb_type_from_data_type(service.field.type)
					if type == tt.type || (tt.type == PB_TYPE.LENGTHDEL && service.field.rule == PB_RULE.REPEATED && service.field.option_packed):
						var res : int = unpack_field(bytes, offset, service.field, type, service.func_ref)
						if res > 0:
							service.state = PB_SERVICE_STATE.FILLED
							offset = res
							if offset == limit:
								return offset
							elif offset > limit:
								return PB_ERR.PACKAGE_SIZE_MISMATCH
						elif res < 0:
							return res
						else:
							break
			else:
				return offset
		return PB_ERR.UNDEFINED_STATE

	static func pack_message(data) -> PoolByteArray:
		var DEFAULT_VALUES
		if PROTO_VERSION == 2:
			DEFAULT_VALUES = DEFAULT_VALUES_2
		elif PROTO_VERSION == 3:
			DEFAULT_VALUES = DEFAULT_VALUES_3
		var result : PoolByteArray = PoolByteArray()
		var keys : Array = data.keys()
		keys.sort()
		for i in keys:
			if data[i].field.value != null:
				if data[i].state == PB_SERVICE_STATE.UNFILLED && typeof(data[i].field.value) == typeof(DEFAULT_VALUES[data[i].field.type]) && data[i].field.value == DEFAULT_VALUES[data[i].field.type]:
					continue
				elif data[i].field.rule == PB_RULE.REPEATED && data[i].field.value.size() == 0:
					continue
				result.append_array(pack_field(data[i].field))
			elif data[i].field.rule == PB_RULE.REQUIRED:
				print("Error: required field is not filled: Tag:", data[i].field.tag)
				return PoolByteArray()
		return result

	static func check_required(data) -> bool:
		var keys : Array = data.keys()
		for i in keys:
			if data[i].field.rule == PB_RULE.REQUIRED && data[i].state == PB_SERVICE_STATE.UNFILLED:
				return false
		return true

	static func construct_map(key_values):
		var result = {}
		for kv in key_values:
			result[kv.get_key()] = kv.get_value()
		return result
	
	static func tabulate(text : String, nesting : int) -> String:
		var tab : String = ""
		for _i in range(nesting):
			tab += DEBUG_TAB
		return tab + text
	
	static func value_to_string(value, field : PBField, nesting : int) -> String:
		var result : String = ""
		var text : String
		if field.type == PB_DATA_TYPE.MESSAGE:
			result += "{"
			nesting += 1
			text = message_to_string(value.data, nesting)
			if text != "":
				result += "\n" + text
				nesting -= 1
				result += tabulate("}", nesting)
			else:
				nesting -= 1
				result += "}"
		elif field.type == PB_DATA_TYPE.BYTES:
			result += "<"
			for i in range(value.size()):
				result += String(value[i])
				if i != (value.size() - 1):
					result += ", "
			result += ">"
		elif field.type == PB_DATA_TYPE.STRING:
			result += "\"" + value + "\""
		elif field.type == PB_DATA_TYPE.ENUM:
			result += "ENUM::" + String(value)
		else:
			result += String(value)
		return result
	
	static func field_to_string(field : PBField, nesting : int) -> String:
		var result : String = tabulate(field.name + ": ", nesting)
		if field.type == PB_DATA_TYPE.MAP:
			if field.value.size() > 0:
				result += "(\n"
				nesting += 1
				for i in range(field.value.size()):
					var local_key_value = field.value[i].data[1].field
					result += tabulate(value_to_string(local_key_value.value, local_key_value, nesting), nesting) + ": "
					local_key_value = field.value[i].data[2].field
					result += value_to_string(local_key_value.value, local_key_value, nesting)
					if i != (field.value.size() - 1):
						result += ","
					result += "\n"
				nesting -= 1
				result += tabulate(")", nesting)
			else:
				result += "()"
		elif field.rule == PB_RULE.REPEATED:
			if field.value.size() > 0:
				result += "[\n"
				nesting += 1
				for i in range(field.value.size()):
					result += tabulate(String(i) + ": ", nesting)
					result += value_to_string(field.value[i], field, nesting)
					if i != (field.value.size() - 1):
						result += ","
					result += "\n"
				nesting -= 1
				result += tabulate("]", nesting)
			else:
				result += "[]"
		else:
			result += value_to_string(field.value, field, nesting)
		result += ";\n"
		return result
		
	static func message_to_string(data, nesting : int = 0) -> String:
		var DEFAULT_VALUES
		if PROTO_VERSION == 2:
			DEFAULT_VALUES = DEFAULT_VALUES_2
		elif PROTO_VERSION == 3:
			DEFAULT_VALUES = DEFAULT_VALUES_3
		var result : String = ""
		var keys : Array = data.keys()
		keys.sort()
		for i in keys:
			if data[i].field.value != null:
				if data[i].state == PB_SERVICE_STATE.UNFILLED && typeof(data[i].field.value) == typeof(DEFAULT_VALUES[data[i].field.type]) && data[i].field.value == DEFAULT_VALUES[data[i].field.type]:
					continue
				elif data[i].field.rule == PB_RULE.REPEATED && data[i].field.value.size() == 0:
					continue
				result += field_to_string(data[i].field, nesting)
			elif data[i].field.rule == PB_RULE.REQUIRED:
				result += data[i].field.name + ": " + "error"
		return result



############### USER DATA BEGIN ################


class Detection:
	func _init():
		var service
		
		_label = PBField.new("label", PB_DATA_TYPE.STRING, PB_RULE.REPEATED, 1, false, [])
		service = PBServiceField.new()
		service.field = _label
		data[_label.tag] = service
		
		_label_id = PBField.new("label_id", PB_DATA_TYPE.INT32, PB_RULE.REPEATED, 2, true, [])
		service = PBServiceField.new()
		service.field = _label_id
		data[_label_id.tag] = service
		
		_score = PBField.new("score", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 3, true, [])
		service = PBServiceField.new()
		service.field = _score
		data[_score.tag] = service
		
		_location_data = PBField.new("location_data", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 4, false, DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _location_data
		service.func_ref = funcref(self, "new_location_data")
		data[_location_data.tag] = service
		
		_feature_tag = PBField.new("feature_tag", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 5, false, DEFAULT_VALUES_2[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _feature_tag
		data[_feature_tag.tag] = service
		
		_track_id = PBField.new("track_id", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 6, false, DEFAULT_VALUES_2[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _track_id
		data[_track_id.tag] = service
		
		_detection_id = PBField.new("detection_id", PB_DATA_TYPE.INT64, PB_RULE.OPTIONAL, 7, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT64])
		service = PBServiceField.new()
		service.field = _detection_id
		data[_detection_id.tag] = service
		
		_associated_detections = PBField.new("associated_detections", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 8, false, [])
		service = PBServiceField.new()
		service.field = _associated_detections
		service.func_ref = funcref(self, "add_associated_detections")
		data[_associated_detections.tag] = service
		
		_display_name = PBField.new("display_name", PB_DATA_TYPE.STRING, PB_RULE.REPEATED, 9, false, [])
		service = PBServiceField.new()
		service.field = _display_name
		data[_display_name.tag] = service
		
		_timestamp_usec = PBField.new("timestamp_usec", PB_DATA_TYPE.INT64, PB_RULE.OPTIONAL, 10, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT64])
		service = PBServiceField.new()
		service.field = _timestamp_usec
		data[_timestamp_usec.tag] = service
		
	var data = {}
	
	var _label: PBField
	func get_label() -> Array:
		return _label.value
	func clear_label() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_label.value = DEFAULT_VALUES_2[PB_DATA_TYPE.STRING]
	func add_label(value : String) -> void:
		_label.value.append(value)
	
	var _label_id: PBField
	func get_label_id() -> Array:
		return _label_id.value
	func clear_label_id() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_label_id.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
	func add_label_id(value : int) -> void:
		_label_id.value.append(value)
	
	var _score: PBField
	func get_score() -> Array:
		return _score.value
	func clear_score() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_score.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
	func add_score(value : float) -> void:
		_score.value.append(value)
	
	var _location_data: PBField
	func get_location_data() -> LocationData:
		return _location_data.value
	func clear_location_data() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_location_data.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func new_location_data() -> LocationData:
		_location_data.value = LocationData.new()
		return _location_data.value
	
	var _feature_tag: PBField
	func get_feature_tag() -> String:
		return _feature_tag.value
	func clear_feature_tag() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_feature_tag.value = DEFAULT_VALUES_2[PB_DATA_TYPE.STRING]
	func set_feature_tag(value : String) -> void:
		_feature_tag.value = value
	
	var _track_id: PBField
	func get_track_id() -> String:
		return _track_id.value
	func clear_track_id() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_track_id.value = DEFAULT_VALUES_2[PB_DATA_TYPE.STRING]
	func set_track_id(value : String) -> void:
		_track_id.value = value
	
	var _detection_id: PBField
	func get_detection_id() -> int:
		return _detection_id.value
	func clear_detection_id() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_detection_id.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT64]
	func set_detection_id(value : int) -> void:
		_detection_id.value = value
	
	var _associated_detections: PBField
	func get_associated_detections() -> Array:
		return _associated_detections.value
	func clear_associated_detections() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_associated_detections.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func add_associated_detections() -> Detection.AssociatedDetection:
		var element = Detection.AssociatedDetection.new()
		_associated_detections.value.append(element)
		return element
	
	var _display_name: PBField
	func get_display_name() -> Array:
		return _display_name.value
	func clear_display_name() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_display_name.value = DEFAULT_VALUES_2[PB_DATA_TYPE.STRING]
	func add_display_name(value : String) -> void:
		_display_name.value.append(value)
	
	var _timestamp_usec: PBField
	func get_timestamp_usec() -> int:
		return _timestamp_usec.value
	func clear_timestamp_usec() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_timestamp_usec.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT64]
	func set_timestamp_usec(value : int) -> void:
		_timestamp_usec.value = value
	
	class AssociatedDetection:
		func _init():
			var service
			
			_id = PBField.new("id", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _id
			data[_id.tag] = service
			
			_confidence = PBField.new("confidence", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _confidence
			data[_confidence.tag] = service
			
		var data = {}
		
		var _id: PBField
		func get_id() -> int:
			return _id.value
		func clear_id() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_id.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_id(value : int) -> void:
			_id.value = value
		
		var _confidence: PBField
		func get_confidence() -> float:
			return _confidence.value
		func clear_confidence() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_confidence.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_confidence(value : float) -> void:
			_confidence.value = value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class DetectionList:
	func _init():
		var service
		
		_detection = PBField.new("detection", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 1, false, [])
		service = PBServiceField.new()
		service.field = _detection
		service.func_ref = funcref(self, "add_detection")
		data[_detection.tag] = service
		
	var data = {}
	
	var _detection: PBField
	func get_detection() -> Array:
		return _detection.value
	func clear_detection() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_detection.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func add_detection() -> Detection:
		var element = Detection.new()
		_detection.value.append(element)
		return element
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class LocationData:
	func _init():
		var service
		
		_format = PBField.new("format", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _format
		data[_format.tag] = service
		
		_bounding_box = PBField.new("bounding_box", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _bounding_box
		service.func_ref = funcref(self, "new_bounding_box")
		data[_bounding_box.tag] = service
		
		_relative_bounding_box = PBField.new("relative_bounding_box", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _relative_bounding_box
		service.func_ref = funcref(self, "new_relative_bounding_box")
		data[_relative_bounding_box.tag] = service
		
		_mask = PBField.new("mask", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 4, false, DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _mask
		service.func_ref = funcref(self, "new_mask")
		data[_mask.tag] = service
		
		_relative_keypoints = PBField.new("relative_keypoints", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 5, false, [])
		service = PBServiceField.new()
		service.field = _relative_keypoints
		service.func_ref = funcref(self, "add_relative_keypoints")
		data[_relative_keypoints.tag] = service
		
	var data = {}
	
	var _format: PBField
	func get_format():
		return _format.value
	func clear_format() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_format.value = DEFAULT_VALUES_2[PB_DATA_TYPE.ENUM]
	func set_format(value) -> void:
		_format.value = value
	
	var _bounding_box: PBField
	func get_bounding_box() -> LocationData.BoundingBox:
		return _bounding_box.value
	func clear_bounding_box() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_bounding_box.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func new_bounding_box() -> LocationData.BoundingBox:
		_bounding_box.value = LocationData.BoundingBox.new()
		return _bounding_box.value
	
	var _relative_bounding_box: PBField
	func get_relative_bounding_box() -> LocationData.RelativeBoundingBox:
		return _relative_bounding_box.value
	func clear_relative_bounding_box() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_relative_bounding_box.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func new_relative_bounding_box() -> LocationData.RelativeBoundingBox:
		_relative_bounding_box.value = LocationData.RelativeBoundingBox.new()
		return _relative_bounding_box.value
	
	var _mask: PBField
	func get_mask() -> LocationData.BinaryMask:
		return _mask.value
	func clear_mask() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_mask.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func new_mask() -> LocationData.BinaryMask:
		_mask.value = LocationData.BinaryMask.new()
		return _mask.value
	
	var _relative_keypoints: PBField
	func get_relative_keypoints() -> Array:
		return _relative_keypoints.value
	func clear_relative_keypoints() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_relative_keypoints.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func add_relative_keypoints() -> LocationData.RelativeKeypoint:
		var element = LocationData.RelativeKeypoint.new()
		_relative_keypoints.value.append(element)
		return element
	
	enum Format {
		GLOBAL = 0,
		BOUNDING_BOX = 1,
		RELATIVE_BOUNDING_BOX = 2,
		MASK = 3
	}
	
	class BoundingBox:
		func _init():
			var service
			
			_xmin = PBField.new("xmin", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _xmin
			data[_xmin.tag] = service
			
			_ymin = PBField.new("ymin", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _ymin
			data[_ymin.tag] = service
			
			_width = PBField.new("width", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _width
			data[_width.tag] = service
			
			_height = PBField.new("height", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 4, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _height
			data[_height.tag] = service
			
		var data = {}
		
		var _xmin: PBField
		func get_xmin() -> int:
			return _xmin.value
		func clear_xmin() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_xmin.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_xmin(value : int) -> void:
			_xmin.value = value
		
		var _ymin: PBField
		func get_ymin() -> int:
			return _ymin.value
		func clear_ymin() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_ymin.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_ymin(value : int) -> void:
			_ymin.value = value
		
		var _width: PBField
		func get_width() -> int:
			return _width.value
		func clear_width() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_width.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_width(value : int) -> void:
			_width.value = value
		
		var _height: PBField
		func get_height() -> int:
			return _height.value
		func clear_height() -> void:
			data[4].state = PB_SERVICE_STATE.UNFILLED
			_height.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_height(value : int) -> void:
			_height.value = value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	class RelativeBoundingBox:
		func _init():
			var service
			
			_xmin = PBField.new("xmin", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _xmin
			data[_xmin.tag] = service
			
			_ymin = PBField.new("ymin", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _ymin
			data[_ymin.tag] = service
			
			_width = PBField.new("width", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _width
			data[_width.tag] = service
			
			_height = PBField.new("height", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 4, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _height
			data[_height.tag] = service
			
		var data = {}
		
		var _xmin: PBField
		func get_xmin() -> float:
			return _xmin.value
		func clear_xmin() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_xmin.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_xmin(value : float) -> void:
			_xmin.value = value
		
		var _ymin: PBField
		func get_ymin() -> float:
			return _ymin.value
		func clear_ymin() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_ymin.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_ymin(value : float) -> void:
			_ymin.value = value
		
		var _width: PBField
		func get_width() -> float:
			return _width.value
		func clear_width() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_width.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_width(value : float) -> void:
			_width.value = value
		
		var _height: PBField
		func get_height() -> float:
			return _height.value
		func clear_height() -> void:
			data[4].state = PB_SERVICE_STATE.UNFILLED
			_height.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_height(value : float) -> void:
			_height.value = value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	class BinaryMask:
		func _init():
			var service
			
			_width = PBField.new("width", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _width
			data[_width.tag] = service
			
			_height = PBField.new("height", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _height
			data[_height.tag] = service
			
			_rasterization = PBField.new("rasterization", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE])
			service = PBServiceField.new()
			service.field = _rasterization
			service.func_ref = funcref(self, "new_rasterization")
			data[_rasterization.tag] = service
			
		var data = {}
		
		var _width: PBField
		func get_width() -> int:
			return _width.value
		func clear_width() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_width.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_width(value : int) -> void:
			_width.value = value
		
		var _height: PBField
		func get_height() -> int:
			return _height.value
		func clear_height() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_height.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_height(value : int) -> void:
			_height.value = value
		
		var _rasterization: PBField
		func get_rasterization() -> Rasterization:
			return _rasterization.value
		func clear_rasterization() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_rasterization.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
		func new_rasterization() -> Rasterization:
			_rasterization.value = Rasterization.new()
			return _rasterization.value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	class RelativeKeypoint:
		func _init():
			var service
			
			_x = PBField.new("x", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _x
			data[_x.tag] = service
			
			_y = PBField.new("y", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _y
			data[_y.tag] = service
			
			_keypoint_label = PBField.new("keypoint_label", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.STRING])
			service = PBServiceField.new()
			service.field = _keypoint_label
			data[_keypoint_label.tag] = service
			
			_score = PBField.new("score", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 4, false, DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _score
			data[_score.tag] = service
			
		var data = {}
		
		var _x: PBField
		func get_x() -> float:
			return _x.value
		func clear_x() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_x.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_x(value : float) -> void:
			_x.value = value
		
		var _y: PBField
		func get_y() -> float:
			return _y.value
		func clear_y() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_y.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_y(value : float) -> void:
			_y.value = value
		
		var _keypoint_label: PBField
		func get_keypoint_label() -> String:
			return _keypoint_label.value
		func clear_keypoint_label() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_keypoint_label.value = DEFAULT_VALUES_2[PB_DATA_TYPE.STRING]
		func set_keypoint_label(value : String) -> void:
			_keypoint_label.value = value
		
		var _score: PBField
		func get_score() -> float:
			return _score.value
		func clear_score() -> void:
			data[4].state = PB_SERVICE_STATE.UNFILLED
			_score.value = DEFAULT_VALUES_2[PB_DATA_TYPE.FLOAT]
		func set_score(value : float) -> void:
			_score.value = value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Rasterization:
	func _init():
		var service
		
		_interval = PBField.new("interval", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 1, false, [])
		service = PBServiceField.new()
		service.field = _interval
		service.func_ref = funcref(self, "add_interval")
		data[_interval.tag] = service
		
	var data = {}
	
	var _interval: PBField
	func get_interval() -> Array:
		return _interval.value
	func clear_interval() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_interval.value = DEFAULT_VALUES_2[PB_DATA_TYPE.MESSAGE]
	func add_interval() -> Rasterization.Interval:
		var element = Rasterization.Interval.new()
		_interval.value.append(element)
		return element
	
	class Interval:
		func _init():
			var service
			
			_y = PBField.new("y", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 1, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _y
			data[_y.tag] = service
			
			_left_x = PBField.new("left_x", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 2, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _left_x
			data[_left_x.tag] = service
			
			_right_x = PBField.new("right_x", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 3, false, DEFAULT_VALUES_2[PB_DATA_TYPE.INT32])
			service = PBServiceField.new()
			service.field = _right_x
			data[_right_x.tag] = service
			
		var data = {}
		
		var _y: PBField
		func get_y() -> int:
			return _y.value
		func clear_y() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_y.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_y(value : int) -> void:
			_y.value = value
		
		var _left_x: PBField
		func get_left_x() -> int:
			return _left_x.value
		func clear_left_x() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_left_x.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_left_x(value : int) -> void:
			_left_x.value = value
		
		var _right_x: PBField
		func get_right_x() -> int:
			return _right_x.value
		func clear_right_x() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_right_x.value = DEFAULT_VALUES_2[PB_DATA_TYPE.INT32]
		func set_right_x(value : int) -> void:
			_right_x.value = value
		
		func to_string() -> String:
			return PBPacker.message_to_string(data)
			
		func to_bytes() -> PoolByteArray:
			return PBPacker.pack_message(data)
			
		func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
			var cur_limit = bytes.size()
			if limit != -1:
				cur_limit = limit
			var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
			if result == cur_limit:
				if PBPacker.check_required(data):
					if limit == -1:
						return PB_ERR.NO_ERRORS
				else:
					return PB_ERR.REQUIRED_FIELDS
			elif limit == -1 && result > 0:
				return PB_ERR.PARSE_INCOMPLETE
			return result
		
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
################ USER DATA END #################
