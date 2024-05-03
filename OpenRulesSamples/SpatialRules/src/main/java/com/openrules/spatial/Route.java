package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class Route extends GeoEntity {
	
	public Route(int i, Geometry geometry) {
		super(i,geometry);
	}

	@Override
	public String getType() {
		return "Route";
	}

	@Override
	public String toString() {
		return "Route [getId()=" + getId() + ", getName()=" + getName()
				+ ", getGeometry()=" + getGeometry() + ", isSpatial()="
				+ isSpatial() 
				+ "]";
	}
	
	

}
