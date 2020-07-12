#version $GLSL_VERSION_STR
$GLSL_DEFAULT_PRECISION_FLOAT
 
#pragma vp_name       REX Engine - Fragment
#pragma vp_entryPoint elevation_last_frag
#pragma vp_location   fragment_coloring
#pragma vp_order      0.9

uniform sampler2D tile_xyz;
 
//tile coordinate
vec4 oe_layer_tilec;

// SDK functions:
float oe_terrain_getElevation();
 
float linearstep(float edge0, float edge1, float x) {
    float t = (x - edge0)/(edge1 - edge0);
    return clamp(t, 0.0, 1.0);
}
 
void elevation_last_frag(inout vec4 color)
{	
	
	vec4 tilexyz = texture(tile_xyz, oe_layer_tilec.st);
	if(tilexyz.w>0)
		color = tilexyz;
	
	
	vec4 vec4ColorArray[64];
	vec4ColorArray[ 0] = vec4(0.00000, 0.00000,   0.56250,     1.0);
  vec4ColorArray[ 1] = vec4(0.00000, 0.00000,   0.62500,     1.0);
  vec4ColorArray[ 2] = vec4(0.00000, 0.00000,   0.68750,     1.0);
  vec4ColorArray[ 3] = vec4(0.00000, 0.00000,   0.75000,     1.0);
  vec4ColorArray[ 4] = vec4(0.00000, 0.00000,   0.81250,     1.0);
  vec4ColorArray[ 5] = vec4(0.00000, 0.00000,   0.87500,     1.0);
  vec4ColorArray[ 6] = vec4(0.00000, 0.00000,   0.93750,     1.0);
  vec4ColorArray[ 7] = vec4(0.00000, 0.00000,   1.00000,     1.0);
  vec4ColorArray[ 8] = vec4(0.00000, 0.06250,   1.00000,     1.0);
  vec4ColorArray[ 9] = vec4(0.00000, 0.12500,   1.00000,     1.0);
  vec4ColorArray[10] = vec4(0.00000, 0.18750,   1.00000,     1.0);
  vec4ColorArray[11] = vec4(0.00000, 0.25000,   1.00000,     1.0);
  vec4ColorArray[12] = vec4(0.00000, 0.31250,   1.00000,     1.0);
  vec4ColorArray[13] = vec4(0.00000, 0.37500,   1.00000,     1.0);
  vec4ColorArray[14] = vec4(0.00000, 0.43750,   1.00000,     1.0);
  vec4ColorArray[15] = vec4(0.00000, 0.50000,   1.00000,     1.0);
  vec4ColorArray[16] = vec4(0.00000, 0.56250,   1.00000,     1.0);
  vec4ColorArray[17] = vec4(0.00000, 0.62500,   1.00000,     1.0);
  vec4ColorArray[18] = vec4(0.00000, 0.68750,   1.00000,     1.0);
  vec4ColorArray[19] = vec4(0.00000, 0.75000,   1.00000,     1.0);
  vec4ColorArray[20] = vec4(0.00000, 0.81250,   1.00000,     1.0);
  vec4ColorArray[21] = vec4(0.00000, 0.87500,   1.00000,     1.0);
  vec4ColorArray[22] = vec4(0.00000, 0.93750,   1.00000,     1.0);
  vec4ColorArray[23] = vec4(0.00000, 1.00000,   1.00000,     1.0);
  vec4ColorArray[24] = vec4(0.06250, 1.00000,   0.93750,     1.0);
  vec4ColorArray[25] = vec4(0.12500, 1.00000,   0.87500,     1.0);
  vec4ColorArray[26] = vec4(0.18750, 1.00000,   0.81250,     1.0);
  vec4ColorArray[27] = vec4(0.25000, 1.00000,   0.75000,     1.0);
  vec4ColorArray[28] = vec4(0.31250, 1.00000,   0.68750,     1.0);
  vec4ColorArray[29] = vec4(0.37500, 1.00000,   0.62500,     1.0);
  vec4ColorArray[30] = vec4(0.43750, 1.00000,   0.56250,     1.0);
  vec4ColorArray[31] = vec4(0.50000, 1.00000,   0.50000,     1.0);
  vec4ColorArray[32] = vec4(0.56250, 1.00000,   0.43750,     1.0);
  vec4ColorArray[33] = vec4(0.62500, 1.00000,   0.37500,     1.0);
  vec4ColorArray[34] = vec4(0.68750, 1.00000,   0.31250,     1.0);
  vec4ColorArray[35] = vec4(0.75000, 1.00000,   0.25000,     1.0);
  vec4ColorArray[36] = vec4(0.81250, 1.00000,   0.18750,     1.0);
  vec4ColorArray[37] = vec4(0.87500, 1.00000,   0.12500,     1.0);
  vec4ColorArray[38] = vec4(0.93750, 1.00000,   0.06250,     1.0);
  vec4ColorArray[39] = vec4(1.00000, 1.00000,   0.00000,     1.0);
  vec4ColorArray[40] = vec4(1.00000, 0.93750,   0.00000,     1.0);
  vec4ColorArray[41] = vec4(1.00000, 0.87500,   0.00000,     1.0);
  vec4ColorArray[42] = vec4(1.00000, 0.81250,   0.00000,     1.0);
  vec4ColorArray[43] = vec4(1.00000, 0.75000,   0.00000,     1.0);
  vec4ColorArray[44] = vec4(1.00000, 0.68750,   0.00000,     1.0);
  vec4ColorArray[45] = vec4(1.00000, 0.62500,   0.00000,     1.0);
  vec4ColorArray[46] = vec4(1.00000, 0.56250,   0.00000,     1.0);
  vec4ColorArray[47] = vec4(1.00000, 0.50000,   0.00000,     1.0);
  vec4ColorArray[48] = vec4(1.00000, 0.43750,   0.00000,     1.0);
  vec4ColorArray[49] = vec4(1.00000, 0.37500,   0.00000,     1.0);
  vec4ColorArray[50] = vec4(1.00000, 0.31250,   0.00000,     1.0);
  vec4ColorArray[51] = vec4(1.00000, 0.25000,   0.00000,     1.0);
  vec4ColorArray[52] = vec4(1.00000, 0.18750,   0.00000,     1.0);
  vec4ColorArray[53] = vec4(1.00000, 0.12500,   0.00000,     1.0);
  vec4ColorArray[54] = vec4(1.00000, 0.06250,   0.00000,     1.0);
  vec4ColorArray[55] = vec4(1.00000, 0.00000,   0.00000,     1.0);
  vec4ColorArray[56] = vec4(0.93750, 0.00000,   0.00000,     1.0);
  vec4ColorArray[57] = vec4(0.87500, 0.00000,   0.00000,     1.0);
  vec4ColorArray[58] = vec4(0.81250, 0.00000,   0.00000,     1.0);
  vec4ColorArray[59] = vec4(0.75000, 0.00000,   0.00000,     1.0);
  vec4ColorArray[60] = vec4(0.68750, 0.00000,   0.00000,     1.0);
  vec4ColorArray[61] = vec4(0.62500, 0.00000,   0.00000,     1.0);
  vec4ColorArray[62] = vec4(0.56250, 0.00000,   0.00000,     1.0);
  vec4ColorArray[63] = vec4(0.50000, 0.00000,   0.00000,     1.0);
	
	//snow height
	float snowHeight=5000.0;
 

	
	
	//AntiAliasing
	float border=0.02;
	vec4 lineColor=vec4(1.0,0.0,0.0,1.0);
	float step,m;
	if(oe_layer_tilec.s<border){	
		step = linearstep(0,border,oe_layer_tilec.s);	
		m =  smoothstep(0.0,1.0,step);
		color = mix(lineColor,color,vec4(m,m,m,m));
	}
	if(oe_layer_tilec.s>1-border){	
		step = linearstep(1-border,1,oe_layer_tilec.s);	
		m =  1-smoothstep(0.0,1.0,step);
		color = mix(lineColor,color,vec4(m,m,m,m));
	}
	if(oe_layer_tilec.t<border){	
		step = linearstep(0,border,oe_layer_tilec.t);	
		m =  smoothstep(0.0,1.0,step);
		color = mix(lineColor,color,vec4(m,m,m,m));
	}
	if(oe_layer_tilec.t>1-border){	
		step = linearstep(1-border,1,oe_layer_tilec.t);	
		m =  1-smoothstep(0.0,1.0,step);
		color = mix(lineColor,color,vec4(m,m,m,m));
	}
	
}