#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Spot light data
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};

// Material data
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Point lights being used in the scene
uniform point_light points[4];
// Spot lights being used in the scene
uniform spot_light spots[5];
// Material of the object being rendered
uniform material mat;
// Position of the eye (camera)
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

// Point light calculation
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour) {
   // *********************************
  // Get distance between point light and vertex
 float d=distance(point.position,position);

  // Calculate attenuation factor
 float kds=point.constant+(point.linear*d)+(point.quadratic*pow(d,2));
  vec4 att=(1/kds)*point.light_colour;

  // Calculate light colour
  vec4 light_colour=point.light_colour*att;

  // Calculate light dir
  vec3 dir=normalize(point.position-position);

  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
     // Calculate diffuse component

   // Calculate k
  float k = max(dot(normal, dir), 0.0);

  // Calculate diffuse
  vec4 diffuse = k * (mat.diffuse_reflection * light_colour);

  // Calculate half vector
  vec3 half_vector=normalize(dir+view_dir);

  // Calculate specular component
  float n_h=dot(normal,half_vector);
  float max_n_h=max(n_h,0.0f);
  float specular_intensity=pow(max_n_h,mat.shininess);
  vec4 specular=specular_intensity*mat.specular_reflection*light_colour;

  // Calculate primary colour component
  vec4 primary=mat.emissive+diffuse;

  // Calculate final colour - remember alpha
  vec4 secondary=specular;

   //set alphas
  primary.a = 1.0f;
  secondary.a=1.0f;
  light_colour.a=1.0f;

  //calculate colour
  colour=primary*tex_colour+secondary;  
  return colour;
}

// Spot light calculation
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour) {
  vec3 dir=normalize(spot.position-position);

  // Calculate distance to light
 float d=distance(spot.position,position);

  // Calculate attenuation value
  float kds=(spot.constant+(spot.linear*d)+(spot.quadratic*pow(d,2)));
  vec4 att=(1/kds)*spot.light_colour;

  // Calculate spot light intensity
  vec4 spot_intensity=att*pow(max(dot(spot.direction,-dir),0),spot.power);

  // Calculate light colour
	vec4 light_colour=spot.light_colour;


  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
     // Calculate diffuse component
   // Calculate k
  float k = max(dot(normal, dir), 0.0);

  // Calculate diffuse
  vec4 diffuse = k * (mat.diffuse_reflection * light_colour);

  // Calculate half vector
  vec3 half_vector=normalize(dir+view_dir);

  // Calculate specular component
  float n_h=dot(normal,half_vector);
  float max_n_h=max(n_h,0.0f);
  float specular_intensity=pow(max_n_h,mat.shininess);
  vec4 specular=specular_intensity*mat.specular_reflection*light_colour;

  // Calculate primary colour component
  vec4 primary=mat.emissive+diffuse;

  // Calculate final colour - remember alpha
  vec4 secondary=specular;

  //set alphas
  primary.a = 1.0f;
  secondary.a = 1.0f;
  light_colour.a=1.0f;

  //calculate colour
   colour=primary*tex_colour+secondary;


  return colour;
}

void main() {

  colour = vec4(0.0, 0.0, 0.0, 1.0);
  // *********************************
  // Calculate view direction
  vec3 view_dir;
  // Sample texture
  vec4 tex_colour=texture(tex,tex_coord);

  // Sum point lights
    for ( int i = 0; i < 4; ++i){
	 colour += calculate_point ( points [i], mat , position , normal ,view_dir , tex_colour );
	 }

  // Sum spot lights
    for ( int i = 0; i < 5; ++i){
	 colour += calculate_spot ( spots [i], mat , position , normal ,view_dir , tex_colour );
}

}