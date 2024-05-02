package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

//Notice that there is no reference to the Facility parent: 
//this is a JavaBean, not a persistence class (Hibernate, JPA, etc).


public class Window {
	
	private String windowId = null;
	private String replacementCode = null;
	
	public Window() {
	}
	
	public String getWindowId() {
		return windowId;
	}
	public void setWindowId(String windowId) {
		this.windowId = windowId;
	}
	public String getReplacementCode() {
		return replacementCode;
	}
	public void setReplacementCode(String replacementCode) {
		this.replacementCode = replacementCode;
	}

}
