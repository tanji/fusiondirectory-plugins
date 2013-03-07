<?php

/*
 * This code is part of GOsa (http://www.gosa-project.org)
 * Copyright (C) 2009-2010 Bas Tichelaar
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

class zarafaAccount extends simplePlugin
{
  var $displayHeader = TRUE;
  var $objectclasses = array("gosaMailAccount", "zarafa-user");
  /*!
   * \brief  ACL settings
   */
  static function plInfo()
  {
    return (array(
          "plShortName"     => _("Mail"),
          "plDescription"   => _("Zarafa settings"),
          "plSelfModify"    => TRUE,
          "plDepends"       => array("user"),                     // This plugin depends on
          "plPriority"      => 4,                                 // Position in tabs
          "plSection"       => array("personal" => _("My account")),
          "plCategory"      => array("users"),
          "plProvidedAcls"  => parent::generatePlProvidedAcls(self::getAttributesInfo())
        ));
  }


  /*!
   * \brief Constructor of the freeradiusAccount
   *        It will build the select groups choices
   */
  function __construct(&$config, $dn = NULL, $object = NULL)
  {
    parent::__construct($config, $dn, $object);
    $ldap = $this->config->get_ldap_link();
    $ldap->cd($this->config->current['BASE']);
    $ldap->search("(objectClass=goMailServer)",array('cn'));
    $servercn = array();
    while ($attrs = $ldap->fetch()) {
        $servercn[] = $attrs['cn'][0];
    }
    $this->attributesInfo['section1']['attrs']['gosaMailServer']->setChoices($servercn);
    $this->attributesInfo['section2']['attrs']['zarafaAccount']->setManagedAttributes(
      array(
        'erase' => array(
          FALSE => array (
            'section2' => array(
              'zarafaSharedStoreOnly','zarafaHidden','zarafaAdmin','zarafaResourceType','zarafaResourceCapacity',
            )
          )
        )
      )
    );
    $this->attributesInfo['section2']['attrs']['zarafaSharedStoreOnly']->setManagedAttributes(
      array(
        'erase' => array(
          FALSE => array(
            'section2' => array(
              'zarafaAdmin',
            )
          )
        )
      )
    );
  }


  static function getAttributesInfo ()
  {
    return array(
      // Attributes are grouped by section
      'section1' => array(
        'name'  => _('Generic'),
        'attrs' => array(
          new StringAttribute (
            _('Primary address'),
            _('The primary mail address'),
            'mail',
            TRUE,
            '',
            '',
            "/^[^@]*@[^@]*$/"
          ),
          new SelectAttribute (
            _('Server'),
            _('Server mail for zarafa'),
            'gosaMailServer',
            TRUE,
            array("")
          ),
          new IntAttribute (
            _('Hard quota size'),                    // Label
            _(''),  // Description
            'zarafaQuotaHard',                   // LDAP name
            FALSE,                          // Not mandatory
            0,                              // [FloatAttribute] Minimum value
            FALSE,                          // [FloatAttribute] No maximum value
            ''                              // [FloatAttribute] Default value
          ),
          new IntAttribute (
            _('Soft quota size'),                    // Label
            _(''),  // Description
            'zarafaQuotaSoft',                   // LDAP name
            FALSE,                          // Not mandatory
            0,                              // [FloatAttribute] Minimum value
            FALSE,                          // [FloatAttribute] No maximum value
            ''                              // [FloatAttribute] Default value
          ),
          new IntAttribute (
            _('Warn quota size'),                    // Label
            _(''),                          // Description
            'zarafaQuotaWarn',                   // LDAP name
            FALSE,                          // Not mandatory
            0,                              // [FloatAttribute] Minimum value
            FALSE,                          // [FloatAttribute] No maximum value
            ''                              // [FloatAttribute] Default value
          ),
          new SetAttribute (
            new StringAttribute (
              _('Alternative addresses'),
              _('Alternative mail addresses for zarafa group'),
              'gosaMailAlternateAddress',
              FALSE,
              ''
            )
          ),
          new MailAttribute (
            _('Forward messages to non group members'),
            _(''),
            'gosaMailForwardingAddress',
            FALSE,
            ''
          )
        )
      ),
      'section2' => array (
        'name'  => _('Zarafa specific settings'),
        'attrs' => array(
          new BooleanAttribute (
            _('Enable Zarafa'),
            _('Enable Zarafa Account for this user'),
            'zarafaAccount',
            FALSE,
            FALSE,
            '',
            1,
            0
          ),
          new BooleanAttribute (
            _('Zarafa Administrator'),
            _('Make this user a Zarafa Administrator'),
            'zarafaAdmin',
            FALSE,
            FALSE,
            '',
            1,
            0
          ),
          new BooleanAttribute (
            _('Zarafa shared store'),
            _('Make this user a Zarafa shared store'),
            'zarafaSharedStoreOnly',
            FALSE,
            FALSE,
            '',
            1,
            0
          ),
          new BooleanAttribute (
            _('Hide from addressbook'),
            _('Hide from Zarafa addressbook'),
            'zarafaHidden',
            FALSE,
            FALSE,
            '',
            1,
            0
          ),
          new SelectAttribute (
            _('Resource type'),
            _(''),  // Description
            'zarafaResourceType',
            FALSE,
            array('user','room','equipment'),
            "",
            array(_('User'),_('Room'),_('Equipment'))
          ),
          new IntAttribute (
            _('Resource capacity'),
            _(''),  // Description
            'zarafaResourceCapacity',
            FALSE,
            0,
            FALSE,
            ''
          ),
          new MailAttribute (
            _('Send as privileges'),
            _(''),
            'zarafaSendAsPrivilege',
            FALSE,
            ''
          )
        )
      )
    );
  }
}