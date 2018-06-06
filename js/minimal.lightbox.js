var imagesArray = document.getElementsByTagName('img');
	
function minimalLightbox() {
	var imageSelect = this.src;
	if(this.getAttribute('minimal-lightbox') != '')
		imageSelect = this.getAttribute('minimal-lightbox');
	var topLayer = document.createElement('div');
	var displayImage = document.createElement('div');
	var closeAll = document.createElement('a');

	topLayer.setAttribute('id', 'top-layer');
	document.body.appendChild(topLayer);

	displayImage.setAttribute('id', 'display-image');
	document.body.appendChild(displayImage);

	closeAll.setAttribute('id', 'close');
	displayImage.appendChild(closeAll);
	closeAll.innerHTML = '[X]';

	displayImage.addEventListener('click', closeImage);
	window.addEventListener('keydown', closeImage);
	closeAll.addEventListener('click', closeImage);

	topLayer.setAttribute('style', 'height: 100%; width: 100%; top: 0; left: 0; position: fixed; background-color: rgba(0, 0, 0, 0.9);');
	displayImage.setAttribute('style','position: fixed; left: 5%; top: 5%; width: 90%; height: 90%; background-image: url("' + imageSelect + '");  background-size: contain; background-position: center center; background-repeat: no-repeat;');
	closeAll.setAttribute('style', 'position: fixed; right: 20px; top: 20px; font-size: 20px; color: #FFF; cursor: pointer;');
	body = document.getElementsByTagName("body")[0];
	body.setAttribute('style', 'height: 100%; width: 100%; overflow: hidden;');

	window.addEventListener('touchmove', closeImage);
}
function closeImage() {
	body = document.getElementsByTagName('body')[0];
	body.removeAttribute('style');
	imageDelete = document.getElementById('display-image');
	imageDelete.parentNode.removeChild(imageDelete);
	layerDelete = document.getElementById('top-layer');
	layerDelete.parentNode.removeChild(layerDelete);
}
function preload() {
	for(var i = 0; i < imagesArray.length; i++) {
		if(imagesArray[i].hasAttribute('preload') && imagesArray[i].hasAttribute('minimal-lightbox')) {
			var createImage = new Image();
			if(imagesArray[i].getAttribute('minimal-lightbox') != '') {
				createImage.src = imagesArray[i].getAttribute('minimal-lightbox');
			}
		}
	}
}
for (var i = 0; i < imagesArray.length; i++) {
	if(imagesArray[i].hasAttribute('minimal-lightbox')) {
		imagesArray[i].addEventListener('click', minimalLightbox, false);
	}
}
preload();
