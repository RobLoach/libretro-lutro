displayed = false

return {
	draw = function()
		if not displayed then
			displayed = lutro.window.showMessageBox('Title', 'Hello Wrold!')
		end
	end
}
