<template>
	<div>

		<button @click="scanOrStop()"><text v-if="scaning">停止</text>
			<text v-else>扫描</text></button>
		<uni-collapse>
			<uni-collapse-item v-for="(model, i) in deviceArray"
				:title="(model.device.name == undefined ? '' : model.device.name) + '  ' + (model.device.address == undefined ? '' : model.device.address) + ' RSSI:' + model.rssi"
				:key="i">
				<view><button @click="connectOrDisconnect(i)"><text v-if="model.isConnected">断开</text>
						<text v-else>连接</text></button></view>
				<view>广播数据:</view>
				<view>manufacturerId:{{model.advertisementData.manufacturerId}}</view>
				<view>manufacturerSpecificData:{{model.advertisementData.manufacturerSpecificData}}</view>
				<view>serviceUUIDs:{{ JSON.stringify(model.serviceUUIDs)}}</view>
				<view>serviceData:{{ JSON.stringify(model.serviceData)}}</view>

                 <view class="borderView"></view>
				<view v-for="(service, j) in model.services">
					
					<view class="service">服务Service({{j}})</view>
					<view> UUID:{{service.uuid}}</view>
					<view v-if="service.type == 0">服务类型:主服务</view>
					<view v-else>服务类型:主服务</view>
					 <view class="characteristicBorderView"></view>
					<view v-for="(characteristic, k) in service.characteristics">
						<view class="characteristic">特征Characteristic({{k}})</view>
						<view> UUID:{{characteristic.uuid}}</view>
						<view>properties: {{characteristic.properties}} permissions: {{characteristic.permissions}}
						</view>

						<text v-if="(characteristic.properties & 2) > 0">
							<text @click="read(i, j, k)" class="chatacteristicOpt">Read</text>
						</text>
						<text v-else></text>
						<text v-if="(characteristic.properties & 4) > 0">
							<text @click="writeNoResponse(i, j, k)" class="chatacteristicOpt">Write No Response</text>
						</text>
						<text v-else></text>
						<text v-if="(characteristic.properties & 8) > 0">
							<text @click="write(i, j, k)" class="chatacteristicOpt">Write</text>
						</text>
						<text v-else></text>
						<text v-if="(characteristic.properties & 16) > 0">
							<text @click="notify(i, j, k)" class="chatacteristicOpt">Notify</text>
						</text>
						<text v-else></text>
						<text v-if="(characteristic.properties & 32) > 0">
							<text @click="indicate(i, j, k)" class="chatacteristicOpt">Indicate</text>
						</text>
						<text v-else></text>
						 <view class="characteristicBorderView"></view>
					</view>
					<view class="borderView"></view>
				</view>

			</uni-collapse-item>
		</uni-collapse>
		<uni-popup ref="popup">
			<view>十六进制 0x</view>
			<uni-easyinput v-model="value" placeholder="请输入内容"></uni-easyinput>
			<button @click="valueConfirm">确定</button>
		</uni-popup>
		{{msg}}
	</div>
</template>

<script>
	// #ifdef  APP-PLUS
	var bluetoothcenter = uni.requireNativePlugin("wrs-bluetoothcenter");
	// #endif
	import {
		intToByte,
		getSendData,
		hexStrToArrayBuffer,
		arrayBufferToHexStr,
		getCheckingInData,
		intArray2ArrayBuffer,
		addHexStrToArray,
		getOrder,
		encodeCrc8
	} from '@/utils/data_utils.js';
	import {
		isString,
		isObject,
		isType
	} from '@/utils/is_type.js'


	export default {
		data() {
			var deviceArray = [];

			// deviceArray.push({
			// 	device: {
			// 		deviceId: "54354",
			// 		name: "ios",
			// 		address: "45:ss:ss"
			// 	},
			// 	rssi: -2,
			// 	advertisementData: {
			// 		manufacturerId: 12,
			// 		manufacturerSpecificData: "1352412"
			// 	},
			// 	services: [{
			// 		uuid: "121200-435435-sss",
			// 		type: 0,
			// 		characteristics: [{
			// 			uuid: "asdfaf-4154-4241",
			// 			properties: 26,
			// 			permissions: 2
			// 		}]
			// 	}, {
			// 		uuid: "121200-435435-sss",
			// 		type: 0,
			// 		characteristics: [{
			// 			uuid: "asdfaf-4154-4241",
			// 			properties: 26,
			// 			permissions: 2
			// 		}]
			// 	}, {
			// 		uuid: "121200-435435-sss",
			// 		type: 0,
			// 		characteristics: [{
			// 			uuid: "asdfaf-4154-4241",
			// 			properties: 26,
			// 			permissions: 2
			// 		}, {
			// 			uuid: "asdfaf-4154-4241",
			// 			properties: 26,
			// 			permissions: 2
			// 		},{
			// 			uuid: "asdfaf-4154-4241",
			// 			properties: 26,
			// 			permissions: 2
			// 		}]
			// 	}]
			// });


			return {
				scaning: false,
				deviceArray: deviceArray,
				value: null,
				valueConfirmCallback: null,
				msg: null
			}
		},
		onUnload: function() {
			// 释放资源
bluetoothcenter.releaseResource();
		},
		onLoad: function() {
			// #ifdef  APP-PLUS

			switch (uni.getSystemInfoSync().platform) {
				case 'android':
					// 申请动态权限
					this.requestPermission();
					break;
				default:
					break;
			}
			// 设置蓝牙回调
			bluetoothcenter.setCenterCallback((resp) => {
				var opt = resp.opt;
				switch (opt) {
					// 蓝牙状态改变，仅支持iOS
					case "centralManagerDidUpdateState":
					     // state:
						// 0: CBManagerStateUnknown
						// 1: CBManagerStateResetting
						// 2: CBManagerStateUnsupported
						// 3: CBManagerStateUnauthorized
						// 4: CBManagerStatePoweredOff
						// 5: CBManagerStatePoweredOn
					  var state = resp;
					break;
					// 扫描发现蓝牙设备
					case "onScanBluetooth":
						this.deviceArray.push(resp);
						break;
						// 蓝牙连接成功
					case "onConnectSuccess":
						var deviceId = resp.device.deviceId;
						var bluetooth = this.getBluetooth(deviceId);
						this.$set(bluetooth, "isConnected", true);
						// 连接成功后，开始查找service和characteristic
						this.$set(bluetooth, "services", []);
						bluetoothcenter.findServiceAndCharacteristic({
							"deviceId": deviceId
						});
						break;
						// 蓝牙连接失败
					case "onConnectFail":
						var deviceId = resp.device.deviceId;
						var bluetooth = this.getBluetooth(deviceId);
						this.$set(bluetooth, "isConnected", false);
						this.$set(bluetooth, "services", []);
						break;
						// 蓝牙断开连接
					case "onDisConnected":
						var deviceId = resp.device.deviceId;
						var bluetooth = this.getBluetooth(deviceId);
						this.$set(bluetooth, "isConnected", false);
						this.$set(bluetooth, "services", []);
						break;
						// 发现characteristic
					case "onFindCharacteristic":
						var deviceId = resp.device.deviceId;
						var bluetooth = this.getBluetooth(deviceId);
						bluetooth.services.push(resp.service);
						break;
						// 收到数据
					case "onCharacteristicWriteRequest":
						var str = JSON.stringify(resp);
						this.showMsg("收到数据:" + str);
						break;
						// 写数据成功,即成功发送数据
					case "onWriteSuccess":
						this.showMsg("写数据成功");
						break;
						// 写数据失败,即发送数据失败
					case "onWriteFailure":
						this.showMsg("写数据失败");
						break;
						// 读数据成功
					case "onReadSuccess":
						this.showMsg("读数据成功");
						break;
						// 读数据失败
					case "onReadFailure":
						this.showMsg("读数据失败");
						break;
					default:
						break;
				}
			});
			// 初始化
			var params = {};
			switch (uni.getSystemInfoSync().platform) {
				case 'ios':
					params.CBCentralManagerOptionShowPowerAlertKey = true;
					// params.CBCentralManagerOptionRestoreIdentifierKey = "xxx";
					break;
				default:
					break;
			}
			bluetoothcenter.initBluetoothCenter(params);
			// #endif
		},
		methods: {
			scanOrStop: function() {
				this.scaning = !this.scaning;
				if (this.scaning) {
					this.deviceArray.length = 0;
					var params = {};
					// params.filterName = "slave"; // 过滤蓝牙名
					switch (uni.getSystemInfoSync().platform) {
						case 'ios':
							params.CBCentralManagerScanOptionAllowDuplicatesKey = false; // 是否运行重复扫描已经发现的设备，仅支持iOS
							break;
							case 'android':
							// params.mac = "xx"; // mac地址，仅支持Android
							break;
						default:
							break;
					}
					// params.services = ["0000fff1-0000-1000-8000-00805f9b34fb"]; // 服务UUID
					
					// params.timeout = 10000; // 扫描时间，毫秒
					bluetoothcenter.startScanBluetooth(params);
				} else {
					bluetoothcenter.stopScanBluetooth();
				}
			},
			getBluetooth: function(deviceId) {
				var length = this.deviceArray.length;
				for (var i = 0; i < length; i++) {
					var bluetooth = this.deviceArray[i];
					if (deviceId == bluetooth.device.deviceId) {
						return bluetooth;
					}
				}
				return null;
			},
			connectOrDisconnect: function(i) {
				// 先停止扫描
				bluetoothcenter.stopScanBluetooth();

				let bluetooth = this.deviceArray[i];
				var resp = bluetoothcenter.isConnected({
					"deviceId": bluetooth.device.deviceId
				});
				if (isString(resp)) {
					resp = JSON.parse(resp);
				}
				if (resp.isConnected) {
					// 断开连接
					bluetoothcenter.disConnectBluetooth({
						"deviceId": bluetooth.device.deviceId
					});
				} else {
					// 连接蓝牙设备
					
					bluetoothcenter.connectBluetooth({
						"deviceId": bluetooth.device.deviceId
					});
				}
			},
			read: function(i, j, k) {
				var bluetooth = this.deviceArray[i];
				var service = bluetooth.services[j];
				var characteristic = service.characteristics[k];
				bluetoothcenter.readData({
					"deviceId": bluetooth.device.deviceId,
					"serviceUuid": service.uuid,
					"characteristicUuid": characteristic.uuid
				});
			},
			writeNoResponse: function(i, j, k) {
				this.showInputValue(function(value) {
					var bluetooth = this.deviceArray[i];
					var service = bluetooth.services[j];
					var characteristic = service.characteristics[k];
					bluetoothcenter.writeData({
						"deviceId": bluetooth.device.deviceId,
						"serviceUuid": service.uuid,
						"characteristicUuid": characteristic.uuid,
						"value": value,
						"dataType": -1, // 发送数据类型， -1: 不进行转换，以原始数据发送 0: 以十六进制发送，此时data数据为十六进制字符串或整型数组 1:以字符编码发送，此时data数据为字符串
						"charsetName": "UTF_8", // 字符编码， 当dataType为1时生效，常用编码为：ISO_8859_1、US_ASCII、UTF_16、UTF_16BE、UTF_16LE、UTF_8
						"writeType": 0 // 写类型,仅支持iOS， 0: CBCharacteristicWriteWithResponse 1: CBCharacteristicWriteWithoutResponse
					});
				});
			},
			write: function(i, j, k) {
				this.showInputValue(function(value) {
					var bluetooth = this.deviceArray[i];
					var service = bluetooth.services[j];
					var characteristic = service.characteristics[k];
					bluetoothcenter.writeData({
						"deviceId": bluetooth.device.deviceId,
						"serviceUuid": service.uuid,
						"characteristicUuid": characteristic.uuid,
						"value": value,
						"writeType": 0 // 写类型,仅支持iOS， 0: CBCharacteristicWriteWithResponse 1: CBCharacteristicWriteWithoutResponse
						
					});
				});
			},
			notify: function(i, j, k) {
				var bluetooth = this.deviceArray[i];
				var service = bluetooth.services[j];
				var characteristic = service.characteristics[k];
				bluetoothcenter.openNotify({
					"deviceId": bluetooth.device.deviceId,
					"serviceUuid": service.uuid,
					"characteristicUuid": characteristic.uuid
				}, (resp)=> {
					if(resp.flag) { // 打开成功
						
					} else { // 打开失败
						
					}
				});
			},
			indicate: function(i, j, k) {

			},
			requestPermission: function() {
				plus.android.requestPermissions(
					[
						"android.permission.BLUETOOTH_SCAN",
						"android.permission.BLUETOOTH_ADVERTISE",
						"android.permission.BLUETOOTH_CONNECT",
						"android.permission.ACCESS_COARSE_LOCATION",
						"android.permission.ACCESS_FINE_LOCATION"
					],
					function(resultObj) {
						for (var i = 0; i < resultObj.granted.length; i++) {
							var grantedPermission = resultObj.granted[i];
							console.log('已获取的权限：' + grantedPermission);
						}
						for (var i = 0; i < resultObj.deniedPresent.length; i++) {
							var deniedPresentPermission = resultObj.deniedPresent[i];
							console.log('拒绝本次申请的权限：' + deniedPresentPermission);
						}
						for (var i = 0; i < resultObj.deniedAlways.length; i++) {
							var deniedAlwaysPermission = resultObj.deniedAlways[i];
							console.log('永久拒绝申请的权限：' + deniedAlwaysPermission);
						}
						// 若所需权限被永久拒绝,则打开APP设置界面,可以在APP设置界面打开相应权限  
						if (resultObj.deniedAlways.length > 0) {
							// var Intent = plus.android.importClass("android.content.Intent");
							// var Settings = plus.android.importClass("android.provider.Settings");
							// var Uri = plus.android.importClass("android.net.Uri");
							// var mainActivity = plus.android.runtimeMainActivity();
							// var intent = new Intent();
							// intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
							// var uri = Uri.fromParts("package", mainActivity.getPackageName(), null);
							// intent.setData(uri);
							// mainActivity.startActivity(intent);
						}
					},
					function(error) {
						console.log('申请权限错误：' + error.code + " = " + error.message);
					});
			},
			showInputValue: function(callback) {
				this.value = null;
				this.valueConfirmCallback = callback;
				this.$refs.popup.open('center')
			},
			valueConfirm: function() {
				this.$refs.popup.close();
				if (this.valueConfirmCallback) {
					this.valueConfirmCallback(this.value);
				}
			},
			clear: function() {
				this.msg = null;
			},
			showMsg: function(info) {
				if (this.msg) {
					this.msg = this.msg + "\n" + info;
				} else {
					this.msg = info;
				}
			}

		}
	}
</script>

<style>
	.section {}

	.service {
		font-size: 110%;

	}

	.characteristic {
		font-size: 110%;


	}

	.chatacteristicOpt {
		margin-left: 10rpx;
		color: red;
	}
	.borderView {
		height: 1rpx;
		background-color: aquamarine;
	}
	.characteristicBorderView {
		height: 1rpx;
		background-color: red;
	}
</style>
