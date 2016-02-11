library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity vga_top is
    Port ( clk : in  STD_LOGIC;
           btn : in  STD_LOGIC_VECTOR (3 downto 0);
           sw : in  STD_LOGIC_VECTOR (7 downto 0);
           Hsync : out  STD_LOGIC;
           Vsync : out  STD_LOGIC;
           vgaRed : out  STD_LOGIC_VECTOR (2 downto 0);
           vgaGreen : out  STD_LOGIC_VECTOR (2 downto 0);
           vgaBlue : out  STD_LOGIC_VECTOR (1 downto 0);
			  seg : out STD_LOGIC_VECTOR (6 downto 0); --exercise 3
			  dp : out std_Logic; 							--exercise 3
			  an : out std_logic_vector (3 downto 0) --exercise 3
			  );
end vga_top;

architecture Behavioral of vga_top is

	component vga_timing
		port(
		clk : in  STD_LOGIC;
		  rst : in  STD_LOGIC;
		  HS : out  STD_LOGIC;
		  VS : out  STD_LOGIC;
		  pixel_x : out  STD_LOGIC_VECTOR (9 downto 0);
		  pixel_y : out  STD_LOGIC_VECTOR (9 downto 0);
		  last_column : out  STD_LOGIC;
		  last_row : out  STD_LOGIC;
		  blank : out  STD_LOGIC
		  );
	end component;
		
	signal top_pixel_x, top_pixel_y : std_logic_vector (9 downto 0);
	signal top_last_row, top_last_column, top_blank, Hnext, Vnext : std_logic;
	signal red_next, red, green_next, green : std_logic_vector (2 downto 0);
	signal blue_next, blue : std_logic_vector (1 downto 0);
	
	signal red_bar, green_bar : std_logic_vector (2 downto 0); --vertical bar signals
	signal blue_bar: std_logic_vector (1 downto 0);
	
	signal rectangle1_on, rectangle2_on, rectangle3_on, rectangle4_on : std_logic; --exercise 2
	signal rectangle_color : std_logic_Vector (7 downto 0); -- exercise 2
	
	signal s, g :std_logic; -- custom object
	signal letter_color : std_logic_vector (7 downto 0);
	
	signal frame_count,frame_count_next : unsigned (15 downto 0) := (others => '0'); --exercise 3
	
begin
	main : vga_timing
	port map(
	clk, btn(3), Hnext , Vnext, top_pixel_x, top_pixel_y,
	top_last_column , top_last_row , top_blank 
	);

	--registers
	process(clk)
	begin
		if (clk'event and clk = '1') then
			vgaRed <= red_next;
			vgaGreen <= green_next;
			vgaBlue <= blue_next;
			Hsync <= Hnext;
			Vsync <= Vnext;
			if (top_last_row'event and top_last_column'event and top_last_row = '1' and top_last_column = '1) then
				frame_count <= frame_count_next;
			end if;
		end if;
	end process;
	
	--VERTICAL BAR LOGIC
	red_bar <= "000" when unsigned(top_pixel_x) < 320 else
				"111";
	green_bar <= "000" when unsigned(top_pixel_x) < 160 else
				"111" when unsigned(top_pixel_x) < 320 else
				"000" when unsigned(top_pixel_x) < 480 else
				"111";
	blue_bar <= "00" when unsigned(top_pixel_x) < 80 else
				"11" when unsigned(top_pixel_x) < 160 else
				"00" when unsigned(top_pixel_x) < 240 else
				"11" when unsigned(top_pixel_x )< 320 else
				"00" when unsigned(top_pixel_x )< 400 else
				"11" when unsigned(top_pixel_x )< 480 else
				"00" when unsigned(top_pixel_x )< 560 else
				"11";

	--OBJECTS
	rectangle1_on <= '1' when unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 280
								and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 200 else
							'0';
	rectangle2_on <= '1' when unsigned(top_pixel_x) >= 400 and unsigned(top_pixel_x) <= 500
								and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 200 else
							'0';
	rectangle3_on <= '1' when unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 280
								and unsigned(top_pixel_y) >= 300 and unsigned(top_pixel_y) <= 400 else
							'0';
	rectangle4_on <= '1' when unsigned(top_pixel_x) >= 400 and unsigned(top_pixel_x) <= 500
								and unsigned(top_pixel_y) >= 300 and unsigned(top_pixel_y) <= 400 else
							'0';
							
	--rectangle_color 
	rectangle_color <= "11100000" when rectangle1_on = '1' else --all read
							"00011100" when rectangle2_on = '1' else --green
							"11111100" when rectangle3_on = '1' else --yellow
							"11100011" when rectangle4_on = '1' else--magenta
							"11111111"; -- all white
							
	--my custom objects
	s <= '1' when (unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 280 --top of the s
				and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 110)
				or
				(unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 190 --left side of s
				and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 150 )
				or
				(unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 280 -- middle of s
				and unsigned(top_pixel_y) >= 150 and unsigned(top_pixel_y) <= 160)
				or
				(unsigned(top_pixel_x) >= 270 and unsigned(top_pixel_x) <= 280 -- right of s
				and unsigned(top_pixel_y) >= 150 and unsigned(top_pixel_y) <= 200 )
				or
				(unsigned(top_pixel_x) >= 180 and unsigned(top_pixel_x) <= 280 -- bottom of s
				and unsigned(top_pixel_y) >= 190 and unsigned(top_pixel_y) <= 200 ) 	else
			'0';
	g <= '1' when (unsigned(top_pixel_x) >= 300 and unsigned(top_pixel_x) <= 400 --top of the G
				and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 110)
				or
				(unsigned(top_pixel_x) >= 300 and unsigned(top_pixel_x) <= 310 --left of g
				and unsigned(top_pixel_y) >= 100 and unsigned(top_pixel_y) <= 200)
				or
				(unsigned(top_pixel_x) >= 300 and unsigned(top_pixel_x) <= 400 -- bottom of g
				and unsigned(top_pixel_y) >= 190 and unsigned(top_pixel_y) <= 200)
				or
				(unsigned(top_pixel_x) >= 390 and unsigned(top_pixel_x) <= 400 --right of g
				and unsigned(top_pixel_y) >= 150 and unsigned(top_pixel_y) <= 200)
				or
				(unsigned(top_pixel_x) >= 305 and unsigned(top_pixel_x) <= 400 --middle of g
				and unsigned(top_pixel_y) >= 150 and unsigned(top_pixel_y) <= 160) else
			'0';
				
	
	--custom object color
	letter_color <= "10000010" when s = '1' or g = '1' else --hopefully purple
							"00000000"; 
	
	
	--pixel color logic
	red <= rectangle_color(7 downto 5) when btn(0) = '1' else -- the rectangles
			letter_color(7 downto 5) when btn(1) = '1' else -- my custom
			sw(7 downto 5) when btn(2) = '1' else -- switches
			red_bar; --vertical bars
				
	green <=  rectangle_color(4 downto 2) when btn(0) = '1' else -- the rectangles
			letter_color(4 downto 2) when btn(1) = '1' else  -- my custom
			sw(4 downto 2) when btn(2) = '1' else
			green_bar; --vertical bars
					
	blue <= rectangle_color(1 downto 0) when btn(0) = '1' else -- the rectangles
			letter_color(1 downto 0) when btn(1) = '1' else -- my custom
			sw(1 downto 0) when btn(2) = '1' else --switches
			blue_bar; --vertical bars
	
	--blank
	red_next <= red when top_blank = '0' else "000";
	green_next <= green when top_blank = '0' else "000";
	blue_next <= blue when top_blank = '0' else "00";
	
	--counter
	frame_count_next <= frame_count + 1;
	--how to attach to seven_seg?

end Behavioral;

