<?php
  /**
   * Schedule
   * Class to create schedule images with the help of a XML file
   *
   * @author  Jeph <a25617@alunos.det.ua.pt>
   * @license GNU General Public License
   * @package Schedule
   * @version 0.1
   * @url     <none>
   **/
  class Schedule
  {
    /**
     * Schedule::width
     * Image width
     *
     * @access public
     * @var    integer
     **/
    var $width = 700;

    /**
     * Schedule::height
     * Image height
     *
     * @access public
     * @var    integer
     **/
    var $height = 300;

    /**
     * Schedule::bg
     * Background settings (color)
     *
     * @access public
     * @var    array
     **/
    var $bg = array();

    /**
     * Schedule::grid
     * Grid settings (color)
     *
     * @access public
     * @var    array
     **/
    var $grid = array();
    
    /**
     * Schedule::schedule
     * Schedule settings (color)
     *
     * @access public
     * @var    array
     **/
    var $schedule = array();
    
    /**
     * Schedule::legend
     * Legend settings (color, font, padding)
     *
     * @access public
     * @var    array
     **/
    var $legend = array();
    
    /**
     * Schedule::block
     * Block settings (color, font, padding, alpha)
     *
     * @access public
     * @var    array
     **/
    var $block = array();

    /**
     * Schedule::struct
     * XML struct
     *
     * @access private
     * @var    array
     **/
    var $struct = array();
    
    /**
     * Schedule::im
     * Image resource
     *
     * @access private
     * @var    resource
     **/
    var $im;

    /**
     * Schedule::Schedule()
     * Class constructor (can load a $xml data/file)
     *
     * @param string  $xml  XML file or data
     * @param boolean $file Indicates whether $xml is a file or not (data)
     **/
    function Schedule($xml = '', $file = true)
    {
      // default settings
      $this->bg['color']   = array(255, 255, 255);
      $this->grid['color'] = array(180, 180, 180);
      $this->schedule['color'] = array(250, 250, 250);
      $this->block['padding_left'] = 3;
      $this->block['padding_top'] = 3;
      $this->block['font_title'] = 3;
      $this->block['font_description'] = 2;
      $this->block['color_border'] = array(250, 230, 180);
      $this->block['color_bg'] = array(220, 200, 150);
      $this->block['color_title'] = array(150, 150, 150);
      $this->block['color_description'] = array(150, 150, 150);
      $this->block['alpha'] = 50;
      $this->block['align_title'] = 'left';
      $this->block['align_description'] = 'left';
      $this->legend['color_bg'] = array(245, 245, 245);
      $this->legend['color_text'] = array(0, 0, 0);
      $this->legend['font'] = 2;
      $this->legend['padding_top'] = 20;
      $this->legend['padding_bottom'] = 0;
      $this->legend['padding_left'] = 50;
      $this->legend['padding_right'] = 0;
      
      if (strlen($xml) > 0)
      {
        if ($file) Schedule::LoadXmlFile($xml);
        else Schedule::LoadXmlData($xml);
      }
    }
    
    /**
     * Schedule::LoadXmlFile()
     * Load data from file
     *
     * @access public
     * @param  string $file Path to filename
     **/
    function LoadXmlFile($file)
    {
      if (($fp = @fopen($file, 'r')) !== false)
      {
        $content = '';
        while (!feof($fp)) $content .= fread($fp, 4096);
        fclose($fp);
        
        return Schedule::LoadXmlData($content);
      }
      else return false;
    }
    
    /**
     * Schedule::LoadXmlData()
     * Parse data
     *
     * @access public
     * @param  string $data XML data
     **/
    function LoadXmlData($data)
    {
      $xml = new ScheduleXml($data);
      
      if (count($xml->struct) > 0)
      {
        $this->struct = $xml->struct;
        
        // clear unwanted tags
        foreach ($this->struct as $k => $struct)
        {
          if ($struct['tag'] != 'SCHEDULE') unset($this->struct[$k]);
          else
          {
            foreach ($struct['childs'] as $k2 => $child)
            {
              if ($child['tag'] != 'DAY' && $child['tag'] != 'SETTINGS')
              {
                unset($this->struct[$k]['childs'][$k2]);
              }
              elseif ($child['tag'] == 'DAY')
              {
                foreach ($child['childs'] as $k3 => $block)
                {
                  if ($block['tag'] != 'BLOCK')
                  {
                    unset($this->struct[$k]['childs'][$k2]['childs'][$k3]);
                  }
                }
                $this->ReorganizeArray($this->struct[$k]['childs'][$k2]['childs']);
              }
            }
            $this->ReorganizeArray($this->struct[$k]['childs']);
          }
        }
        $this->ReorganizeArray($this->struct);
        return true;
      }
      else return false;
    }
    
    /**
     * Schedule::Draw()
     * Draw schedule
     *
     * @access public
     * @param  string  $output_file File to output
     * @param  integer $schedule    Schedule pointer (usualy 0)
     **/
    function Draw($output_file = '', $schedule = 0)
    {
      $this->im = imagecreatetruecolor($this->width, $this->height);
      
      // check settings
      for ($i = 0; $i < count($this->struct); $i++)
      {
        if ($this->struct[$i]['tag'] == 'SCHEDULE')
        {
          for ($j = 0; $j < count($this->struct[$i]['childs']); $j++)
          {
            if ($this->struct[$i]['childs'][$j]['tag'] == 'SETTINGS')
            {
              $this->UpdateSettings($this->struct[$i]['childs'][$j]['childs']);
            }
          }
        }
      }
      
      $this->DrawBackground();
      $this->DrawGrid($schedule);
      $this->DrawDays($schedule);
      
      if (is_dir(dirname($output_file)))
      {
        // save image
        imagepng($this->im, $output_file);
      }
      else
      {
        // output
        header('Content-Type: image/png');
        imagepng($this->im);
      }
      imagedestroy($this->im);
    }
    
    /**
     * Schedule::DrawBackground()
     * Draw schedule background
     *
     * @access private
     **/
    function DrawBackground()
    {
      $this->AllocateColor('im_bg_color', $this->bg['color']);
      imagefilledrectangle($this->im, 0, 0, $this->width, $this->height, $this->im_bg_color);
    }
    
    /**
     * Schedule::DrawGrid()
     * Draw schedule grid according to schedule pointer
     *
     * @access private
     * @param  integer $schedule Schedule pointer
     **/
    function DrawGrid($schedule)
    {
      $this->block_names = explode(',', $this->struct[$schedule]['attrib']['BLOCKS']);
      $this->day_names = explode(',', $this->struct[$schedule]['attrib']['DAYS']);
      $this->horiz_steps = count($this->block_names);
      $this->vert_steps = count($this->day_names);
      $this->horiz_stepsize = ($this->height - $this->legend['padding_top'] - $this->legend['padding_bottom']) / $this->horiz_steps;
      $this->vert_stepsize = ($this->width - $this->legend['padding_left'] - $this->legend['padding_right']) / $this->vert_steps;
      
      // legend & schedule bg
      $this->AllocateColor('im_legend_color_bg', $this->legend['color_bg']);
      $this->AllocateColor('im_grid_color', $this->grid['color']);

      $x1 = $this->legend['padding_left'];
      $x2 = $this->width - 1 - $this->legend['padding_right'];
      $y1 = $this->legend['padding_top'];
      $y2 = $this->height - 1 - $this->legend['padding_bottom'];
      if ($this->legend['padding_top'] > 0)
      {
        $this->DrawRectangle($x1, 0, $x2, $this->legend['padding_top'], $this->im_grid_color, $this->im_legend_color_bg);
      }
      if ($this->legend['padding_bottom'] > 0)
      {
        $this->DrawRectangle($x1, $this->height - 1 - $this->legend['padding_bottom'], $x2, $this->height - 1, $this->im_grid_color, $this->im_legend_color_bg);
      }
      if ($this->legend['padding_left'] > 0)
      {
        $this->DrawRectangle(0, $y1, $this->legend['padding_left'], $y2, $this->im_grid_color, $this->im_legend_color_bg);
      }
      if ($this->legend['padding_right'] > 0)
      {
        $this->DrawRectangle($this->width - 1 - $this->legend['padding_right'], $y1, $this->width - 1, $y2, $this->im_grid_color, $this->im_legend_color_bg);
      }

      $this->AllocateColor('im_schedule_color_bg', $this->schedule['color']);
      $this->DrawRectangle($x1, $y1, $x2, $y2, $this->im_grid_color, $this->im_schedule_color_bg);


      $this->AllocateColor('im_legend_color_text', $this->legend['color_text']);
      
      // horizontal lines
      $offset = ceil($this->legend['padding_left'] / 2);
      $offset2 = $this->width - 1 - round($this->legend['padding_right'] / 2);
      for ($i = 0; $i < $this->horiz_steps; $i++)
      {
        if ($i < $this->horiz_steps)
        {
          $p = $this->legend['padding_top'] + round($i * $this->horiz_stepsize);
          imageline($this->im, 0, $p, $this->width, $p, $this->im_grid_color);
        }
        if ($this->legend['padding_left'] > 0)
        {
          $this->DrawText($offset, $this->legend['padding_top'] + round(($i + .5) * $this->horiz_stepsize), $this->legend['font'], $this->block_names[$i], $this->im_legend_color_text);
        }
        if ($this->legend['padding_right'] > 0)
        {
          $this->DrawText($offset2, $this->legend['padding_top'] + round(($i + .5) * $this->horiz_stepsize), $this->legend['font'], $this->block_names[$i], $this->im_legend_color_text);
        }
      }

      // vertical lines
      $offset = round($this->legend['padding_top'] / 2);
      $offset2 = $this->height - 1 - round($this->legend['padding_bottom'] / 2);
      for ($i = 0; $i < $this->vert_steps; $i++)
      {
        if ($i < $this->vert_steps)
        {
          $p = $this->legend['padding_left'] + round($i * $this->vert_stepsize);
          imageline($this->im, $p, 0, $p, $this->height, $this->im_grid_color);
        }
        if ($this->legend['padding_top'] > 0)
        {
          $this->DrawText($this->legend['padding_left'] + round(($i + .5) * $this->vert_stepsize), $offset, $this->legend['font'], $this->day_names[$i], $this->im_legend_color_text);
        }
        if ($this->legend['padding_bottom'] > 0)
        {
          $this->DrawText($this->legend['padding_left'] + round(($i + .5) * $this->vert_stepsize), $offset2, $this->legend['font'], $this->day_names[$i], $this->im_legend_color_text);
        }
      }
    }
    
    /**
     * Schedule::DrawDays()
     * Draw schedule blocks for all days given the schedule pointer
     *
     * @access private
     * @param  integer $schedule Schedule pointer
     **/
    function DrawDays($schedule)
    {
      $day = &$this->struct[$schedule]['childs'];
      $max = count($this->day_names);
      $maxblocks = count($this->block_names);
      for ($i = 0; $i < count($day); $i++)
      {
        if ($day[$i]['tag'] == 'DAY')
        {
          $this->DrawBlocks($day[$i]['attrib']['NUMBER'] - 1, $day[$i]['childs'], $max, $maxblocks);
        }
      }
    }

    /**
     * Schedule::UpdateSettings()
     * Updates settings from xml array
     *
     * @access private
     * @param  array   $settings Settings array
     **/
    function UpdateSettings(&$settings)
    {
      for ($i = 0; $i < count($settings); $i++)
      {
        if (in_array(strtolower($settings[$i]['tag']), array('block', 'legend', 'grid', 'schedule', 'bg')))
        {
          $this->UpdatePartSettings($settings[$i], strtolower($settings[$i]['tag']));
        }
      }
    }

    /**
     * Schedule::UpdatePartSettings()
     * Updates settings to a part of the schedule class (block, legend, ..)
     *
     * @access private
     * @param  array   $settings      Settings array
     * @param  string  $settings_part Settings section (block, legend, ..)
     **/
    function UpdatePartSettings(&$settings, $settings_part = 'block')
    {
      /*
       * general settings
       * all this IFs statements are to avoid error when display_errors = On
       * (thanks to Guido Fischer)
       */
      $general = $settings['attrib'];
      if (isset($general['BORDERCOLOR']))
        $this->SetVar($settings_part, $this->GetColor($general['BORDERCOLOR']), 'color_border');
      if (isset($general['BGCOLOR']))
        $this->SetVar($settings_part, $this->GetColor($general['BGCOLOR']), 'color_bg');
      if (isset($general['COLOR']))
        $this->SetVar($settings_part, $this->GetColor($general['COLOR']), 'color');
      if (isset($general['FONT']))
        $this->SetVar($settings_part, $general['FONT'], 'font');
      if (isset($general['LEFTPADDING']))
        $this->SetVar($settings_part, $general['LEFTPADDING'], 'padding_left');
      if (isset($general['TOPPADDING']))
        $this->SetVar($settings_part, $general['TOPPADDING'], 'padding_top');
      if (isset($general['RIGHTPADDING']))
        $this->SetVar($settings_part, $general['RIGHTPADDING'], 'padding_right');
      if (isset($general['BOTTOMPADDING']))
        $this->SetVar($settings_part, $general['BOTTOMPADDING'], 'padding_bottom');
      if (isset($general['ALPHA']))
        $this->SetVar($settings_part, $this->GetPercent($general['ALPHA'], 127), 'alpha');

      // child settings
      for ($i = 0; $i < count($settings['childs']); $i++)
      {
        switch ($settings['childs'][$i]['tag'])
        {
          case 'TITLE':
            $this->SetVar($settings_part, $this->GetColor($settings['childs'][$i]['attrib']['COLOR']), 'color_title');
            $this->SetVar($settings_part, $settings['childs'][$i]['attrib']['FONT'], 'font_title');
            $this->SetVar($settings_part, $this->GetAlign($settings['childs'][$i]['attrib']['ALIGN'], array('left', 'center', 'right')), 'align_title');
            break;
          case 'DESCRIPTION':
            $this->SetVar($settings_part, $this->GetColor($settings['childs'][$i]['attrib']['COLOR']), 'color_description');
            $this->SetVar($settings_part, $settings['childs'][$i]['attrib']['FONT'], 'font_description');
            $this->SetVar($settings_part, $this->GetAlign($settings['childs'][$i]['attrib']['ALIGN'], array('left', 'center', 'right')), 'align_description');
       }
      }
    }

    /**
     * Schedule::SetVar()
     * Sets (depending on value) a variable or array position (given a key)
     *
     * @access private
     * @param  string  $varname   Variable name
     * @param  mixed   $setting   Setting value
     * @param  string  $array_key Array key (for $this->varname)
     **/
    function SetVar($varname, $setting, $array_key = '')
    {
      // debug
      // echo "VAR:'{$varname}' SETTING:'{$setting}' KEY:'{$array_key}'<br>\n";
      if (is_array($setting))
      {
        if (count($setting) > 0)
        {
          if ($array_key != '') $this->{$varname}[$array_key] = $setting;
          else $this->$varname = $setting;
        }
      }
      elseif (strlen($setting) > 0 || is_numeric($setting))
      {
        if ($array_key != '') $this->{$varname}[$array_key] = $setting;
        else $this->$varname = $setting;
      }
    }

    /**
     * Schedule::DrawBlocks()
     * Draw a group of blocks in a given day
     *
     * @access private
     * @param  integer $day       Day where blocks should be drawed
     * @param  array   $blocks    Array containing all blocks
     * @param  integer $maxdays   Total days of the schedule
     * @param  integer $maxblocks Total blocks of the schedule
     **/
    function DrawBlocks($day, $blocks, $maxdays, $maxblocks)
    {
      for ($i = 0; $i < count($blocks); $i++)
      {
        $block = $blocks[$i]['attrib'];
        if (!isset($block['AT'])) $block['AT'] = 1;
        if (!isset($block['DAYSPAN'])) $block['DAYSPAN'] = 0;
        if (!isset($block['BLOCKSPAN'])) $block['BLOCKSPAN'] = 0;
        if (!isset($block['DAYOFFSET'])) $block['DAYOFFSET'] = 0;
        if (!isset($block['BLOCKOFFSET'])) $block['BLOCKOFFSET'] = 0;
        list($x1, $y1, $x2, $y2) = $this->GetCoords($day,
                                                    (int) ($block['AT'] - 1),
                                                    $block['DAYSPAN'],
                                                    $block['BLOCKSPAN'],
                                                    $maxdays,
                                                    $maxblocks,
                                                    $block['DAYOFFSET'],
                                                    $block['BLOCKOFFSET']);

        $block_settings = $this->block;
        $this->UpdatePartSettings($blocks[$i]);
        
        $this->AllocateColor('im_block_color_border', $this->block['color_border'], $this->block['alpha']);
        $this->AllocateColor('im_block_color_bg', $this->block['color_bg'], $this->block['alpha']);

        $this->DrawRectangle($x1 + 1, $y1 + 1, $x2 - 1, $y2 - 1, $this->im_block_color_border, $this->im_block_color_bg);

        if (!isset($this->im_block_color_title))
        {
          $this->AllocateColor('im_block_color_title', $this->block['color_title']);
        }
        
        // block title
        switch ($this->block['align_title'])
        {
          case 'center':
            $x = $x1 + round(($x2 - $x1) / 2);
            $y = $y1 + $this->block['padding_top'];
            break;
          case 'right':
            $x = $x2 - $this->block['padding_left'];
            $y = $y1 + $this->block['padding_top'];
            break;
          default: // left
            $x = $x1 + $this->block['padding_left'];
            $y = $y1 + $this->block['padding_top'];
            $this->block['align_title'] = 'left';
        }
        $this->DrawText($x, $y,
                        $this->block['font_title'],
                        $block['NAME'],
                        $this->im_block_color_title,
                        $this->block['align_title'],
                        'top');

        // block description
        if (strlen($blocks[$i]['data']) > 0)
        {
          if (!isset($this->im_block_color_description))
          {
            $this->AllocateColor('im_block_color_description', $this->block['color_description']);
          }

          $this->DrawDescription($x1 + $this->block['padding_left'],
                                 $y1 + $this->block['padding_top']
                                 + imagefontheight($this->block['font_title']) + 1,
                                 $x2 - $this->block['padding_left'],
                                 $y2 - $this->block['padding_top'],
                                 $this->block['font_description'],
                                 str_replace('\n', "\n", $blocks[$i]['data']),
                                 $this->im_block_color_description,
                                 $this->block['align_description']);
        }
        $this->block = $block_settings;
      }
    }
    
    /**
     * Schedule::GetPercent()
     * Given a string and the max value to 100%, it will return the
     * value corresponding to that percentage
     * e.g.: GetPercent('25%', 40) = 10
     *
     * @access private
     * @param  string  $string String containing a percentage
     * @param  integer $max    Value corresponding to 100%
     * @return integer
     **/
    function GetPercent($string, $max)
    {
      if (substr($string, -1, 1) == '%') $string = substr($string, 0, -1);
      if (is_numeric($string)) return round($string * $max / 100);
      else return '';
    }
    
    /**
     * Schedule::GetColor()
     * Transforms an hex RGB string into a PHP RGB array
     *
     * @access private
     * @param  string  $string String containing the color in hex RGB format
     * @return array
     **/
    function GetColor($string)
    {
      if ($string[0] == '#') $string = substr($string, 1);
      switch (strlen($string))
      {
        case 6: // common #rrggbb
          return array(hexdec(substr($string, 0, 2)), hexdec(substr($string, 2, 2)), hexdec(substr($string, 4)));
        case 3: // #rgb
          return array(hexdec($string[0] . $string[0]), hexdec($string[1] . $string[1]), hexdec($string[2] . $string[2]));
        case 1: // gray scale
          $v = hexdec($string . $string);
          return array($v, $v, $v);
        default:
          return array();
      }
    }

    /**
     * Schedule::GetCoords()
     * Returns coordinates of the 4 corners of a block to draw in the image
     *
     * @access private
     * @param  integer $x        X
     * @param  integer $y        Y
     * @param  integer $x_span   X expansion (to other days)
     * @param  integer $y_span   Y expansion (to other blocks)
     * @param  integer $x_max    Total X (total days)
     * @param  integer $y_max    Total Y (total blocks)
     * @param  integer $x_offset X offset
     * @param  integer $y_offset Y offset
     * @return array
     **/
    function GetCoords($x, $y, $x_span, $y_span, $x_max, $y_max, $x_offset, $y_offset)
    {
      $newx = $this->legend['padding_left'] + round(($x + $x_offset) * $this->vert_stepsize);
      $newy = $this->legend['padding_top'] + round(($y + $y_offset) * $this->horiz_stepsize);
      if (($x + $x_span + 1) >= $x_max) $x2 = $this->width - 1 - $this->legend['padding_right'];
      else $x2 = $this->legend['padding_left'] + round($this->vert_stepsize * ($x + $x_offset + $x_span + 1));
      if (($y + $y_span + 1) >= $y_max) $y2 = $this->height - 1 - $this->legend['padding_bottom'];
      else $y2 = $this->legend['padding_top'] + round($this->horiz_stepsize * ($y + $y_offset + $y_span + 1));
      return array($newx, $newy, $x2, $y2);
    }
    
    /**
     * Schedule::GetAlign()
     * Returns the align if it's valid or the first valid align
     *
     * @access private
     * @param  string  $align           String containing the desired align
     * @param  array   $possible_aligns Array containing all the possible aligns
     * @return string
     **/
    function GetAlign($align, $possible_aligns)
    {
      if (in_array(strtolower($align), $possible_aligns)) return strtolower($align);
      else return $possible_aligns[0];
    }

    /**
     * Schedule::DrawDescription()
     * Draws the description of a block given the 4 corners of the
     * block and some more settings
     *
     * @access private
     * @param  integer $x1    X of top left corner
     * @param  integer $y1    Y of top left corner
     * @param  integer $x2    X of bottom right corner
     * @param  integer $y2    Y of bottom right corner
     * @param  integer $font  Font
     * @param  string  $text  Text to draw
     * @param  integer $color Color
     * @param  string  $align Alignment (default is left)
     **/
    function DrawDescription($x1, $y1, $x2, $y2, $font, $text, $color, $align = 'left')
    {
      $lineheight = imagefontheight($font) + 1;
      $maxy = ($y2 - $lineheight);
      $linemaxchars = floor(($x2 - $x1) / imagefontwidth($font));
      $text = explode("\n", $text);
      for ($i = 0; $i < count($text); $i++)
      {
        while ($y1 <= $maxy && strlen($text[$i]) > 0)
        {
          $s = substr($text[$i], 0, $linemaxchars + 1);
          if (strlen($text[$i]) >= $linemaxchars)
          {
            $p = strrpos($s, ' ');
            if ($p !== false)
            {
              $s = substr($s, 0, $p);
              $text[$i] = substr($text[$i], $p + 1);
            }
            else
            {
              $s = substr($text[$i], 0, $linemaxchars);
              $text[$i] = substr($text[$i], $linemaxchars);
            }
          }
          else $text[$i] = '';

          switch ($align)
          {
            case 'center':
              $this->DrawText($x1 + round(($x2 - $x1) / 2), $y1, $font, $s, $color, $align, 'top');
              break;
            case 'right':
              $this->DrawText($x1, $y1, $font, $s, $color, $align, 'top');
              break;
            default:
              $this->DrawText($x1, $y1, $font, $s, $color, $align, 'top');
          }
          $y1 += $lineheight;
        }
      }
    }
    
    /**
     * Schedule::DrawText()
     * Draw text given (x,y) points, font, color, horizontal and
     * vertical align
     *
     * @access private
     * @param  integer $x      X
     * @param  integer $y      Y
     * @param  integer $font   Font
     * @param  string  $text   Text to draw
     * @param  integer $color  Color
     * @param  string  $align  Horizontal alignment (default is center)
     * @param  string  $valign Vertical alignment (default is middle)
     **/
    function DrawText($x, $y, $font, $text, $color, $align = 'center', $valign = 'middle')
    {
      $width = strlen($text) * imagefontwidth($font);
      if ($align == 'right') $x -= $width;
      elseif ($align == 'center') $x -= round($width / 2);
      if ($valign == 'bottom') $y -= imagefontheight($font);
      elseif ($valign == 'middle') $y -= round(imagefontheight($font) / 2);
      imagestring($this->im, $font, $x, $y, $text, $color);
    }
    
    /**
     * Schedule::DrawRectangle()
     * Draws a rectangle given the 4 corner points, border and
     * background color
     *
     * @access private
     * @param  integer $x1          X of top left corner
     * @param  integer $y1          Y of top left corner
     * @param  integer $x2          X of bottom right corner
     * @param  integer $y2          Y of bottom right corner
     * @param  integer $bordercolor Border color
     * @param  integer $bgcolor     Background color
     **/
    function DrawRectangle($x1, $y1, $x2, $y2, $bordercolor, $bgcolor)
    {
      imagefilledrectangle($this->im, $x1, $y1, $x2, $y2, $bgcolor);
      imagerectangle($this->im, $x1, $y1, $x2, $y2, $bordercolor);
    }
    
    /**
     * Schedule::AllocateColor()
     * Allocates a color (if needed) and sets a variable with the
     * index of that color (alpha is added if possible [GD2])
     *
     * @access private
     * @param  string  $varname Variable name
     * @param  array   $color   Array containing RGB color
     * @param  integer $alpha   Transparency: (opaque) 0<->127 (transparent)
     **/
    function AllocateColor($varname, $color, $alpha = 0)
    {
      if (function_exists('imagecolorexactalpha'))
      {
        $c = imagecolorexactalpha($this->im, $color[0], $color[1], $color[2], $alpha);
        if ($c == -1)
        {
          $c = imagecolorallocatealpha($this->im, $color[0], $color[1], $color[2], $alpha);
        }
      }
      else
      {
        $c = imagecolorexact($this->im, $color[0], $color[1], $color[2]);
        if ($c == -1)
        {
          $c = imagecolorallocate($this->im, $color[0], $color[1], $color[2]);
        }
      }
      $this->$varname = $c;
    }
    
    /**
     * Schedule::ReorganizeArray()
     * Resets the indexes of an array
     *
     * @access private
     * @param  array   $array Array to reorganize
     **/
    function ReorganizeArray(&$array)
    {
      $arrk = array_keys($array);
      for ($i = 0; $i < count($arrk); $i++)
      {
        if ($i != $arrk[$i])
        {
          $array[$i] = $array[$arrk[$i]];
          unset($array[$arrk[$i]]);
        }
      }
    }
  }
  
  /**
   * ScheduleXml
   * Used by package Schedule to parse xml data
   *
   * @author  Jeph
   * @license GNU General Public License
   * @package Schedule
   * @version 0.1
   **/
  class ScheduleXml
  {
    /**
     * ScheduleXml::xml
     * XML parser reference
     *
     * @var resource
     **/
    var $xml;

    /**
     * ScheduleXml::lastnode
     * Array of references to every opened node in ScheduleXml::struct
     *
     * @var array
     **/
    var $lastnode = array();
    
    /**
     * ScheduleXml::struct
     * Array containing XML structure of XML data given
     *
     * @var array
     **/
    var $struct = array();

    /**
     * ScheduleXml::ScheduleXml()
     * Class constructor, parses data
     *
     * @param string $xml
     **/
    function ScheduleXml($xml)
    {
      $this->xml = xml_parser_create();
      xml_set_object($this->xml, $this);
      xml_set_element_handler($this->xml, 'tag_open', 'tag_close');
      xml_set_character_data_handler($this->xml, 'cdata');
      $this->lastnode = array(&$this->struct);
      xml_parse($this->xml, $xml);
      xml_parser_free($this->xml);
    }
    
    /**
     * ScheduleXml::tag_open()
     * Used by xml parser to return an open tag
     *
     * @access private
     * @param  resource $parser     Parser resource
     * @param  string   $tag        Tag found
     * @param  array    $attributes Tag attributes
     **/
    function tag_open($parser, $tag, $attributes)
    {
      $c = count($this->lastnode) - 1;
      // add node to array
      $this->lastnode[$c][] = array('tag'    => $tag, 'attrib' => $attributes,
                                    'data'   => '',   'childs' => array());
                                   
      // add reference
      $this->lastnode[] = &$this->lastnode[$c][count($this->lastnode[$c]) - 1]['childs'];
    }
    
    /**
     * ScheduleXml::tag_close()
     * Used by xml parser to return a closed tag
     *
     * @access private
     * @param  resource $parser Parser resource
     * @param  string   $tag    Tag found
     **/
    function tag_close($parser, $tag)
    {
      // remove last reference
      array_pop($this->lastnode);
    }
    
    /**
     * ScheduleXml::cdata()
     * Used by xml parser to return data inside a tag
     *
     * @access private
     * @param  resource $parser Parser resource
     * @param  string   $cdata  Tag data
     **/
    function cdata($parser, $cdata)
    {
      if (strlen(ltrim($cdata)) > 0)
      {
        $p = count($this->lastnode) - 2;
        $this->lastnode[$p][count($this->lastnode[$p]) - 1]['data'] .= ltrim($cdata);
      }
    }
  }
?>
