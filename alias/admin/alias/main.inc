<?php
/*
 * This code is part of FusionDirectory (http://www.fusiondirectory.org)
 * Copyright (C) 2003-2010 GONICUS GmbH
 * Copyright (C) 2010 Antoine Gallavardin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Remove locks created by this plugin */
if ($remove_lock){
  if(session::is_set('aliasManagement')){
      $macl = session::get('aliasManagement');
      $macl->remove_lock();
  }
}

/* Remove this plugin from session */
if ( $cleanup ){
  session::un_set('aliasManagement');
}else{

  /* Create usermanagement object on demand */
  if (!session::is_set('aliasManagement')){
    $aliasManagement= new aliasManagement ($config, $ui);
    session::set('aliasManagement',$aliasManagement);
  }
  $aliasManagement = session::get('aliasManagement');
  $display= $aliasManagement->execute();

  /* Reset requested? */
  if (isset($_GET['reset']) && $_GET['reset'] == 1){
    session::un_set ('aliasManagement');
  }

  /* Show and save dialog */
  session::set('aliasManagement',$aliasManagement);
}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>