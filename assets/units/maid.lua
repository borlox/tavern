
RegisterUnitType("maid", {
	name = "Maid",
	type = "friend",
	model = "assets/image/character/maid.png",
	talk = {
		_default = "hello",
		hello = {
			text = "Hi, im Maggy.\n\nNice to see you here in the tavern.",
			answers = {
				Answer("Hi Maggy", "bye"),
				Answer("Ok", "_close"),
			},
		},
		bye = {
			text = "Ok, see you then!",
			answers = {
				Answer("Bye bye", "_close"),
			},
		},
	},
})
