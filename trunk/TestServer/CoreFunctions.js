//Core Functions, 
//communicate with PARSE


Parse.initialize("eBS1K1uAUmYLZ82pRUeipced87whjFxu4b2Ju1Fn", "XJnob2TLSU4M7oQmXBOzX1qOagJKgVUJDRgzK1l3");

function RefreshAllMatch(roomType, boardSize) {
	//test
	strContent = '--Refresh Match \t:' + roomType;
	return;

	Parse.Cloud.run('refreshAllMatch', {
		roomType: roomType,
		boardSize: boardSize
	}, {
		success: function(result) {
			var x = document.getElementById("result");
			x.value += '\n' + result;
		},
		
		error: function(error) {
			var x = document.getElementById("result");
			x.value += '\n' + JSON.stringify(error);
		}
	});
}


function PushAllMatch(roomType) {
	//test
	strContent = 'Push New \t:' + roomType;
	return;

	Parse.Cloud.run('pushAllMatch', {roomType: roomType}, {
		success: function(result) {
			var x = document.getElementById("result");
			x.value += '\n' + result;
		},
		
		error: function(error) {
			var x = document.getElementById("result");
			x.value += '\n' + JSON.stringify(error);
		}
	});
}


function PushAllResult(roomType) {
	//test
	strContent = '--|--Push Result \t:' + roomType;
	return;

	Parse.Cloud.run('pushAllResult', { roomType: roomType }, {
		success: function(result) {
			var x = document.getElementById("result");
			x.value += '\n' + result;
		},
		
		error: function(error) {
			var x = document.getElementById("result");
			x.value += '\n' + JSON.stringify(error);
		}
	});
}


function RefreshAllResult(roomType) {
	//test
	strContent = '--|--|--Refresh Result \t:' + roomType;
	return;

	Parse.Cloud.run('refreshAllResult', { roomType: roomType }, {
		success: function(result) {
			var x = document.getElementById("result");
			x.value += '\n' + result;
		},
		
		error: function(error) {
			var x = document.getElementById("result");
			x.value += '\n' + JSON.stringify(error);
		}
	});
}
