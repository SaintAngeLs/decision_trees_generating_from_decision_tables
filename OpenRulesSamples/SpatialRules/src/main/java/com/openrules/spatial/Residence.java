package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class Residence extends GeoEntity {
	
	public Residence(int i, Geometry geometry) {
		super(i,geometry);
	}

	@Override
	public String getType() {
		return "Residence";
	}

	@Override
	public String toString() {
		return "Residence [getId()=" + getId() + ", getName()=" + getName()
				+ ", getGeometry()=" + getGeometry() + ", isSpatial()="
				+ isSpatial() + "]";
	}

}
