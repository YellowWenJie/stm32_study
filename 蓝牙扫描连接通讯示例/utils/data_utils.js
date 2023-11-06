function intToByte(n) {
	return n & 0xFF;
}

/**
 * 特殊字符转义
 * @param {*}} array 
 * @param {*} value 
 */
function addSpecialData(array, value) {
	if (value == 0xFA) {
		array.push(intToByte(0x7A));
		array.push(intToByte(0xFC));
	} else if (value == 0xFB) {
		array.push(intToByte(0x7B));
		array.push(intToByte(0xFC));
	} else if (value == 0xFC) {
		array.push(intToByte(0x7C));
		array.push(intToByte(0xFC));
	} else {
		array.push(intToByte(value));
	}
}

/**
 * 通用的socket格式封装
 * @param {*} order int 指令
 * @param {*} data ArrayBuffer 数据，整型数组
 */
function getSendData(order, data) {
	var array = new Array();
	var length = 0;
	var sum = 0;

	if (data != undefined && data != null && data.length > 0) {
		length += data.length;
	}
	length += 3;

	sum += length;
	sum += order;
	if (data != undefined && data != null && data.length > 0) {
		for (let index = 0; index < data.length; index++) {
			const element = data[index];
			sum += element;
		}
	}

	array.push(0xFA);
	array.push(0xFB);
	array.push(intToByte(length));
	addSpecialData(array, order);
	if (data != undefined && data != null && data.length > 0) {
		for (let index = 0; index < data.length; index++) {
			const element = data[index];
			addSpecialData(array, element);
		}
	}

	sum = sum & 0x7F;
	array.push(intToByte(sum));
	var uintArray = new Uint8Array(array)
	var arrayBuffer = uintArray.buffer;
	return arrayBuffer;
}

function intArray2ArrayBuffer(array) {
	var uintArray = new Uint8Array(array)
	var arrayBuffer = uintArray.buffer;
	return arrayBuffer;
}

/**
 * 16进制字符串转ArrayBuffer
 * @param {*} hex String
 */
function hexStrToArrayBuffer(hex) {
	var typedArray = new Uint8Array(hex.match(/[\da-f]{2}/gi).map(function(h) {
		return parseInt(h, 16)
	}));
	var buffer = typedArray.buffer;
	return buffer;
}
/**
 * ArrayButter转16进制字符串
 * @param {*} buffer ArrayBuffer
 */
function arrayBufferToHexStr(buffer) { // buffer is an ArrayBuffer
	return Array.prototype.map.call(new Uint8Array(buffer), x => ('00' + x.toString(16)).slice(-2)).join('');
}

function addHexStrToArray(hexStr, count, array) {
	while (hexStr.length < count * 2) {
		hexStr = "0" + hexStr;
	}
	var end = 0;
	for (let index = 0; index < count; index++) {
		var start = index * 2;
		end += 2;
		var byteStr = hexStr.slice(start, end);
		var value = parseInt(byteStr, 16);
		array.push(intToByte(value));
	}
}

/**
 * 拼装考勤socket通讯格式 
 * @param {*} uid String 用户ID
 * @param {*} segOrder int 课堂考勤：5 宿舍考勤：6 通用考勤：7
 * @param {*} terminalArray Array 格式为[{"name":"020506","level": 1}],如果name以#为前缀，需要删除#
 * @param {*} FDStutas int 是否使用人脸   未使用：0 使用：1 识别失败：2
 * @param {*} FDLevel int 人脸阈值
 * @param {*} groupId String 组ID
 */
function getCheckingInData(uid, segOrder, terminalArray, FDStutas, FDLevel, groupId) {
	var order = 0x49; // 考勤order
	var array = new Array();
	array.push(intToByte(segOrder)); // SegOrd
	array.push(0xFE); // version 版本号
	var uidHexStr = parseInt(uid).toString(16)
	addHexStrToArray(uidHexStr, 8, array); // uid 8 byte
	if (terminalArray != undefined && terminalArray != null && terminalArray.length > 0) {
		array.push(intToByte(terminalArray.length)); // 终端数
		// 按信号强度强到弱拍好顺的终端数据,level绝对值越小表示信号越强
		terminalArray.sort(function(a, b) {
			return Math.abs(a.level) - Math.abs(b.level);
		});
		for (let i = 0; i < terminalArray.length; i++) {
			if (i >= 10) {
				break;
			}
			const terminal = terminalArray[i];
			const name = terminal.name;
			var level = terminal.level;
			addHexStrToArray(name, 9, array); // uid 8 byte
			if (level < 0) {
				level = -level;
			}
			array.push(intToByte(level));
		}
	} else {
		array.push(0x00); // 终端数
	}
	array.push(intToByte(FDStutas));
	array.push(intToByte(FDLevel));
	addHexStrToArray(groupId, 2, array); // group 8 byte
	return getSendData(order, array);
}

function encodeCrc8(hex) {
	var _crc8 = [
		0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
		0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
		0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
		0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
		0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
		0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
		0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
		0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
		0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
		0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
		0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
		0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
		0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
		0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
		0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
		0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
		0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
		0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
		0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
		0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
		0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
		0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
		0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
		0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
		0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
		0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
		0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
		0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
		0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
		0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
		0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
		0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
	];

	var typedArray = new Uint8Array(hex.match(/[\da-f]{2}/gi).map(function(h) {
		return parseInt(h, 16)
	}))

	var ucLen = typedArray.length;

	var ucPtr = typedArray;

	var ucCRC8 = 0;

	var i = 0;

	while (ucLen--)

	{
		ucCRC8 = _crc8[ucCRC8 ^ ucPtr[i]];
		i++;

	}
	return ucCRC8.toString(16).toUpperCase();

}

function getOrder(macArray, sourceIDArray, commamdAccumulate, netID, deviceID, commamdType, dataArray) {
	var array = new Array();
	array.push(0xAA);
	array.push(0xD6);
	array.push(0xBE);
	array.push(0x89);
	array.push(0x8E);
	array.push(0x42);
	array.push(0x1B);

	array.concat(macArray);

	array.push(0x02);
	array.push(0x01);
	array.push(0x1A);
	array.push(0x11);
	array.push(0x07);

	// 源ID
	array.concat(sourceIDArray);

	// 同步字节
	array.push(0x33);

	// 命令累加
	array.push(commamdAccumulate);

	// NETID
	array.push(netID);

	// 设备ID
	array.push(deviceID);

	// 命令类型
	array.push(commamdType);

	// data, data9-data14
	array.concat(dataArray);
	
	var uintArray = new Uint8Array(array)
	var arrayBuffer = uintArray.buffer;
	var hexStr = arrayBufferToHexStr(arrayBuffer);
	var crc8Str = encodeCrc8(hexStr);
	addHexStrToArray(crc8Str, crc8Str.length/2, array); // uid 8 byte
	return array;
}

// var arrayBuffer = foobar.buffer;
// var foobar = new Uint8Array(arrayBuffer);
// var arrayBuffer = foobar.buffer;
module.exports = {
	intToByte: intToByte,
	getSendData: getSendData,
	hexStrToArrayBuffer: hexStrToArrayBuffer,
	arrayBufferToHexStr: arrayBufferToHexStr,
	getCheckingInData: getCheckingInData,
	intArray2ArrayBuffer: intArray2ArrayBuffer,
	addHexStrToArray: addHexStrToArray,
	getOrder: getOrder,
	encodeCrc8: encodeCrc8
}
