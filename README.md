# Contact list viewer
Get actual contact list from device and show contact list with names at the center of each cell.
## Additional features
- [x] Circle contact list pictures at the left side, you may use any available image assets
- [x] Call function: by tap you call ContactListProvider::call(id) and indicate somehow active call
- [x] Grid view switcher for our contact viewer (here contact pictures “must have”)
- [ ] ~~Smart chunk-retrieving for long lists (e.g ContactListProvider has getChunk(index, size))~~
- [x] \(FOR LIST VIEW ONLY) Favorites: possibility to add to favorites (use star/plus button) and show only favorites
- [x] Local filter: text field by using which we may filter out contacts (great if “on the fly”)
- [x] Alphabetical jump: service provides getAvailableLetters chars and sorted contact list
- [ ] ~~Expert level: combine alphabetical jump with previously described chunk retrieving~~
